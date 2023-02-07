#include "mesh.h"
#include <unordered_map>
#include <iostream>
#include "math.h"
#include "glm/geometric.hpp"

// Have a way to hash the Edge class
namespace std{
  template<>
  struct hash<Edge>{
    unsigned int operator()(const Edge& key) const{
      return 100000 * key.a + key.b;
    }
  };
}

bool operator==(const Edge& e1, const Edge& e2){
    return (e1.a == e2.a) and (e1.b == e2.b);
}

void SimpleMesh::boundary(std::vector<Edge> &internalEdges, std::vector<Edge> &externalEdges) const {
    /* Fills the internal edges and external edges vectors. In the internal edges vectors, we order the vector
     * by puting a semi-edge and its inverse next to each other. */

    std::vector<Edge> edges;
    edges.reserve(2 * 3 * triangles.size()); // In the worst case scenario all the mesh will be boudary
    std::unordered_map<Edge, int> in_vector;

    unsigned int externalSize = 0;
    unsigned int internalSize = 0;

    for (size_t i=0; i < triangles.size(); i++){
        const Triangle &t = triangles[i];

        // Edge a, b
        // If the inverse edge has already been added, it adds the current edge next to it
        Edge e = Edge(t.a, t.b, t.c);
        if (in_vector.count(e.reversed())){
            int inverse_edge_index = in_vector[e.reversed()];
            edges[inverse_edge_index + 1] = e;
            internalSize+=2;
        }
        else{
            in_vector[e] = edges.size();
            edges.push_back(e);
            edges.push_back(Edge(-1,-1,-1)); // Add a dummy edge to the list for later removal if it's not overwritten
        }

        // Edge c, a
        e = Edge(t.c, t.a, t.b);
        if (in_vector.count(e.reversed())){
            int inverse_edge_index = in_vector[e.reversed()];
            edges[inverse_edge_index + 1] = e;
            internalSize+=2;
        }
        else{
            in_vector[e] = edges.size();
            edges.push_back(e);
            edges.push_back(Edge(-1,-1,-1));
        }

        // Edge b, c
        e = Edge(t.b, t.c, t.a);
        if (in_vector.count(e.reversed())){
            int inverse_edge_index = in_vector[e.reversed()];
            edges[inverse_edge_index + 1] = e;
            internalSize+=2;
        }
        else{
            in_vector[e] = edges.size();
            edges.push_back(e);
            edges.push_back(Edge(-1,-1,-1));
        }
    }

    externalSize = 3 * triangles.size() - internalSize;

    externalEdges.reserve(externalSize);
    internalEdges.reserve(internalSize);

    // Now we have to remove the dummy edges created by the edge of the mesh where no inverse exist ( the boundary )
    for (size_t i=0; i < edges.size(); i+=2){
        if (edges[i+1].a < 0){ // If the 2nd edge is dummy we have found a external edge with no inverse
            externalEdges.push_back(edges[i]);
        }
        else{
            internalEdges.push_back(edges[i]);
            internalEdges.push_back(edges[i+1]);
        }
    }
}

void CreateGrid(SimpleMesh &m, int nY, int nZ, double step){
    /* Creates a grid mesh in the x plane */

    m.vertices.resize(nY * nZ);
    m.triangles.reserve(2*(nY-1)*(nZ-1));
    // First create the vertices of the mesh
    for (unsigned int j = 0; j < nY; j++){
        for (unsigned int k = 0; k < nZ; k++){
            Vertex v;
            v.Position = glm::vec3(step * j,  0.0f ,step * k);
            v.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
            v.TexCoord = glm::vec2( ( (float) j ) / (nY - 1.0f), ( (float) k ) / (nZ - 1.0f) );
            m.vertices[nZ*j + k] = v;
        }
    }

    // Next we triangulate the mesh
    for (unsigned int j = 0; j < nY-1; j++){
        for (unsigned int k = 0; k < nZ-1; k++){
            // 2 triangles per cell
            Triangle t1;
            Triangle t2;
            t1.a = nZ*j + k;
            t1.b = nZ*j + (k+1);
            t1.c = nZ*(j+1) + k;
            m.triangles.push_back(t1);

            t2.a = nZ*(j+1) + (k+1);
            t2.b = nZ*(j+1) + k;
            t2.c = nZ*j + (k+1);
            m.triangles.push_back(t2);
        }
    }
    for (unsigned int i = 0; i < m.triangles.size(); i++){
        m.indices.push_back(m.triangles[i].a);
        m.indices.push_back(m.triangles[i].b);
        m.indices.push_back(m.triangles[i].c);
    }
    m.createVAO();
}

double SimpleMesh::distance2(int i, int j) const {
    glm::vec3 d = vertices[i].Position - vertices[j].Position;
    return d.x*d.x + d.y*d.y + d.z*d.z;
}

double SimpleMesh::distance(int i, int j) const {
    return sqrt(distance2(i, j));
}

void SimpleMesh::createVAO(){
    const GLenum usage = isDynamic ? GL_DYNAMIC_DRAW: GL_STATIC_DRAW;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload the data of the mesh
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Structs are sequential so we can upload the memory like this
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
                 &indices[0], GL_STATIC_DRAW);

    // Tell the VAO how to interpret the raw vertex data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

    // Unbind the VAO
    glBindVertexArray(0);
}

void SimpleMesh::updateVAO(){
    const GLenum usage = isDynamic ? GL_DYNAMIC_DRAW: GL_STATIC_DRAW;
    glBindVertexArray(VAO);

    // Upload the data of the mesh
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Structs are sequential so we can upload the memory like this
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

    glBindVertexArray(0);
}


void SimpleMesh::bind(){
    glBindVertexArray(VAO);
}

void SimpleMesh::loadFromFile(const std::string &path){
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path,
                                           aiProcess_Triangulate |
                                           aiProcess_GenSmoothNormals |
                                           aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    aiMesh* mesh = scene->mMeshes[0];

    // Vertex attributes
    for (unsigned int i = 0; i < mesh->mNumVertices; i++){
        Vertex vertex;
        glm::vec3 vector;

        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        // Texture coordinates
        if (mesh->mTextureCoords[0]){
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = vec;
        }
        else
            vertex.TexCoord = glm::vec2(0.0f, 0.0f); // no texture coordinates

        vertices.push_back(vertex);
    }

    // Mesh indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    updateTrianglesFromIndices();
    createVAO();
}

void SimpleMesh::updateTrianglesFromIndices(){
    if (indices.size() % 3 != 0){
        std::cout << "ERROR triangulating from indexes" << std::endl;
        return;
    }
    for (unsigned int i = 0; i < indices.size() / 3; i+=3){
        Triangle t;
        t.a = indices[i];
        t.b = indices[i+1];
        t.c = indices[i+2];
    }
}

void SimpleMesh::calculate_normals(){
    // Normals per triangle face
    std::vector<glm::vec3> normals_triangle;
    normals_triangle.resize(indices.size() / 3);

    for (int i=0; i < indices.size(); i+=3){
        const int a = indices[i];
        const int b = indices[i+1];
        const int c = indices[i+2];

        const glm::vec3& ba = vertices[b].Position - vertices[a].Position;
        const glm::vec3& ca = vertices[c].Position - vertices[a].Position;
        glm::vec3 normal = glm::normalize(glm::cross(ba, ca));
        normals_triangle[i/3] = normal;
    }
}

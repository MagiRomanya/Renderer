#include "mesh.h"
#include <iostream>
#include "math.h"
#include "glm/geometric.hpp"

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
            edges.push_back(Edge(-1,-1,-1)); // dummy edge
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

void SimpleMesh::semiEdgeInfo(){
    std::vector<Edge> internal;
    std::vector<Edge> external;

    this->boundary(internal, external);

    for (int i = 0; i < internal.size(); i+=2){
        edge_map[internal[i]] = internal[i+1];
        edge_map[internal[i+1]] = internal[i];
    }
    for (int i = 0; i < external.size(); i++){
        edge_map[external[i]] = Edge(); // dummy edge
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
    m.calculate_vertex_normals();
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
    // Vertex colors
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

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

void processNode(aiNode *node, const aiScene *scene, std::vector<aiMesh*> &meshes){
    for (size_t i = 0; i < node->mNumMeshes; i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(mesh);
    }
    for (size_t i = 0; i < node->mNumChildren; i++){
        processNode(node->mChildren[i], scene, meshes);
    }
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
    std::vector<aiMesh*> meshes;
    processNode(scene->mRootNode, scene, meshes);
    std::cout << "Mesh " << path << " has " << meshes.size() << " meshes" << std::endl;

    for (size_t m = 0; m < meshes.size(); m++){
        aiMesh* mesh = meshes[m];
        unsigned int last_mesh_indices = vertices.size();
        glm::vec3 color = glm::vec3(0.0f);
        if (mesh->HasVertexColors(0)){
            color.r = mesh->mColors[0]->r;
            color.g = mesh->mColors[0]->g;
            color.b = mesh->mColors[0]->b;
        }
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

            // Colors
            vertex.Color = color;

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
                indices.push_back(last_mesh_indices + face.mIndices[j]);
        }
    }
    updateTrianglesFromIndices();
    createVAO();
}

void SimpleMesh::updateTrianglesFromIndices(){
    if (indices.size() % 3 != 0){
        std::cout << "ERROR triangulating from indexes" << std::endl;
        return;
    }
    for (unsigned int i = 0; i < indices.size(); i+=3){
        Triangle t;
        t.a = indices[i];
        t.b = indices[i+1];
        t.c = indices[i+2];
        triangles.push_back(t);
    }
}

void SimpleMesh::updateIndicesFromTriangles(){
    indices.clear();
    indices.reserve(triangles.size()*3);
    for (int i = 0; i < triangles.size(); i++){
        Triangle &t = triangles[i];
        indices.push_back(t.a);
        indices.push_back(t.b);
        indices.push_back(t.c);

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

void SimpleMesh::calculate_vertex_normals(){
    std::vector<float> voronoi_total_area;
    std::vector<std::unordered_map<int, float>> voronoi_area;

    voronoi_area.resize(indices.size()); // num triangles
    voronoi_total_area.resize(indices.size(), 0.0f); // num vertieces

    // Calculate voronoi areas
    for (int i=0; i < indices.size(); i+=3){
        const int a = indices[i];
        const int b = indices[i+1];
        const int c = indices[i+2];

        const float edge_length[] = {
            glm::length(vertices[c].Position - vertices[b].Position), // a -> bc
            glm::length(vertices[a].Position - vertices[c].Position), // b -> ca
            glm::length(vertices[b].Position - vertices[a].Position), // x -> ab
        };

        const int abc[] = {a, b, c};
        const int opposite1[] = {b,c,a};
        const int opposite2[] = {c,a,b};

        for (int j=0; j < 3; j++){
            const glm::vec3& ba = vertices[opposite1[j]].Position - vertices[abc[j]].Position;
            const glm::vec3& ca = vertices[opposite2[j]].Position - vertices[abc[j]].Position;
            float sin_A = glm::cross(ba, ca).length();
            float cos_A = glm::dot(ba, ca);
            float cotan_A = cos_A / sin_A;

            voronoi_area[opposite1[j]][i/3] += 1.0f / 8.0f * edge_length[j] * cotan_A;
            voronoi_area[opposite2[j]][i/3] += 1.0f / 8.0f * edge_length[j] * cotan_A;

            voronoi_total_area[i/3] += 1.0f / 8.0f * edge_length[j] * cotan_A;
            voronoi_total_area[i/3] += 1.0f / 8.0f * edge_length[j] * cotan_A;
        }
    }
    // Set normals to zero
    for (int i=0; i < vertices.size(); i++){ vertices[i].Normal = glm::vec3(0.0f); }

    // Calculate normals
    for (int i=0; i < indices.size(); i+=3){
        const int a = indices[i];
        const int b = indices[i+1];
        const int c = indices[i+2];
        const glm::vec3& ba = vertices[b].Position - vertices[a].Position;
        const glm::vec3& ca = vertices[c].Position - vertices[a].Position;
        const glm::vec3 normal = glm::normalize(glm::cross(ba, ca));

        vertices[a].Normal += voronoi_area[a][i/3] / voronoi_total_area[i/3] * normal;
        vertices[b].Normal += voronoi_area[b][i/3] / voronoi_total_area[i/3] * normal;
        vertices[c].Normal += voronoi_area[c][i/3] / voronoi_total_area[i/3] * normal;
    }
}

void CreateBox(SimpleMesh &m, float dx, float dy, float dz){
    m.vertices.resize(8 * 3);
    m.triangles.resize(12);

    // Generate the vertices
    Vertex vert;
    m.vertices.clear();

    // Face x = 0
    vert.Position = glm::vec3(0.0f, 0.0f, 0.0f);
    vert.Normal = glm::vec3(0.0f ,0.0f, -1.0f);
    vert.TexCoord = glm::vec2(1.0f, 1.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
    vert.TexCoord = glm::vec2(0.0f, 0.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    vert.TexCoord = glm::vec2(0.0f, 1.0f);
    m.vertices.push_back(vert);


    vert.Position = glm::vec3(0.0f, dy, 0.0f);
    vert.Normal = glm::vec3(0.0f ,0.0f, -1.0f);
    vert.TexCoord = glm::vec2(1.0f, 0.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vert.TexCoord = glm::vec2(0.0f, 1.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    vert.TexCoord = glm::vec2(0.0f, 0.0f);
    m.vertices.push_back(vert);

    vert.Position = glm::vec3(0.0f, dy, dz);
    vert.Normal = glm::vec3(0.0f ,0.0f, 1.0f);
    vert.TexCoord = glm::vec2(1.0f, 1.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vert.TexCoord = glm::vec2(1.0f, 1.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    vert.TexCoord = glm::vec2(1.0f, 0.0f);
    m.vertices.push_back(vert);

    vert.Position = glm::vec3(0.0f, 0.0f, dz);
    vert.Normal = glm::vec3(0.0f ,0.0f, 1.0f);
    vert.TexCoord = glm::vec2(1.0f, 0.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
    vert.TexCoord = glm::vec2(1.0f, 1.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    vert.TexCoord = glm::vec2(1.0f, 0.0f);
    m.vertices.push_back(vert);

    // Face x = dx
    vert.Position = glm::vec3(dx, 0.0f, 0.0f);
    vert.Normal = glm::vec3(0.0f ,0.0f, -1.0f);
    vert.TexCoord = glm::vec2(0.0f, 1.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
    vert.TexCoord = glm::vec2(0.0f, 1.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
    vert.TexCoord = glm::vec2(0.0f, 0.0f);
    m.vertices.push_back(vert);


    vert.Position = glm::vec3(dx, dy, 0.0f);
    vert.Normal = glm::vec3(0.0f ,0.0f, -1.0f);
    vert.TexCoord = glm::vec2(0.0f, 0.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vert.TexCoord = glm::vec2(0.0f, 0.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
    vert.TexCoord = glm::vec2(0.0f, 1.0f);
    m.vertices.push_back(vert);

    vert.Position = glm::vec3(dx, dy, dz);
    vert.Normal = glm::vec3(0.0f ,0.0f, 1.0f);
    vert.TexCoord = glm::vec2(0.0f, 1.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vert.TexCoord = glm::vec2(1.0f, 0.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
    vert.TexCoord = glm::vec2(1.0f, 1.0f);
    m.vertices.push_back(vert);

    vert.Position = glm::vec3(dx, 0.0f, dz);
    vert.Normal = glm::vec3(0.0f ,0.0f, 1.0f);
    vert.TexCoord = glm::vec2(0.0f, 0.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
    vert.TexCoord = glm::vec2(1.0f, 1.0f);
    m.vertices.push_back(vert);
    vert.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
    vert.TexCoord = glm::vec2(1.0f, 0.0f);
    m.vertices.push_back(vert);

    // Join the vertices to form the tirangles
    Triangle t;
    m.triangles.clear();
    // Face z positive
    t.a = 3*7;
    t.b = 3*3;
    t.c = 3*2;
    m.triangles.push_back(t);
    t.a = 3*2;
    t.b = 3*6;
    t.c = 3*7;
    m.triangles.push_back(t);

    // Face z negative
    t.a = 3*0;
    t.b = 3*1;
    t.c = 3*4;
    m.triangles.push_back(t);
    t.a = 3*4;
    t.b = 3*1;
    t.c = 3*5;
    m.triangles.push_back(t);

    // Face y positive
    t.a = 3*2 + 1;
    t.b = 3*6 + 1;
    t.c = 3*1 + 1;
    m.triangles.push_back(t);
    t.a = 3*5 + 1;
    t.b = 3*1 + 1;
    t.c = 3*6 + 1;
    m.triangles.push_back(t);

    // Face y negative
    t.a = 3*3 + 1;
    t.b = 3*0 + 1;
    t.c = 3*4 + 1;
    m.triangles.push_back(t);
    t.a = 3*4 + 1;
    t.b = 3*7 + 1;
    t.c = 3*3 + 1;
    m.triangles.push_back(t);

    // Face x positive
    t.a = 3*4 + 2;
    t.b = 3*5 + 2;
    t.c = 3*7 + 2;
    m.triangles.push_back(t);
    t.a = 3*5 + 2;
    t.b = 3*6 + 2;
    t.c = 3*7 + 2;
    m.triangles.push_back(t);

    // Face x negative
    t.a = 3*0 + 2;
    t.b = 3*2 + 2;
    t.c = 3*1 + 2;
    m.triangles.push_back(t);
    t.a = 3*0 + 2;
    t.b = 3*3 + 2;
    t.c = 3*2 + 2;
    m.triangles.push_back(t);

    // Center the vertices so the center of rotation of the cube is in the inside
    for (int i = 0; i < m.vertices.size(); i++){
        Vertex& vert = m.vertices[i];
        vert.Position += glm::vec3(-dx/2.0f, -dy/2.0f, -dz/2.0f);
    }

    m.updateIndicesFromTriangles();
    m.createVAO();
}

glm::vec3 SimpleMesh::aproximate_center() const{
    glm::vec3 center = glm::vec3(0.0f);
    for (int i = 0; i < vertices.size(); i++){
        center += vertices[i].Position;
    }
    return 1.0f / vertices.size() * center;
}

void SimpleMesh::desrtoyVAO(){
    glDeleteVertexArrays(1, &VAO);
}

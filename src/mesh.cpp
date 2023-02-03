#include "mesh.h"
#include <unordered_map>
#include <iostream>
#include "math.h"

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
}

double SimpleMesh::distance2(int i, int j) const {
    glm::vec3 d = vertices[i].Position - vertices[j].Position;
    return d.x*d.x + d.y*d.y + d.z*d.z;
}

double SimpleMesh::distance(int i, int j) const {
    return sqrt(distance2(i, j));
}

void SimpleMesh::createVAO(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload the data of the mesh
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Structs are sequential so we can upload the memory like this
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
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
    glBindVertexArray(VAO);

    // Upload the data of the mesh
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Structs are sequential so we can upload the memory like this
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
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

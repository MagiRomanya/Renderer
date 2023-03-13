#ifndef MESH_H
#define MESH_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>

#include "edge.h"
#include "glad/glad.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

struct Triangle {
    int a, b, c;
};

struct SimpleTexture {
    unsigned int id;
    std::string type;
};

class SimpleMesh {
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Triangle> triangles;

        // Weather or not the vertex positions will change
        bool isDynamic = false;

        std::unordered_map<Edge, Edge> edge_map;

        // Constructor
        SimpleMesh(){}
        SimpleMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
            this->vertices = vertices;
            this->indices = indices;
            this->updateTrianglesFromIndices();
            this->createVAO();
        }

        // Geometry related functions
        void boundary(std::vector<Edge> &internalEdges, std::vector<Edge> &externalEdges) const;

        void semiEdgeInfo();

        double distance(int i, int j) const;

        double distance2(int i, int j) const;

        void calculate_normals();

        void calculate_vertex_normals();

        glm::vec3 aproximate_center() const;

        // OpenGL related functions
        void createVAO();
        void updateVAO();
        void bind();

        void loadFromFile(const std::string &path);

        unsigned int VAO, VBO, EBO;

        void updateIndicesFromTriangles();
        void desrtoyVAO();

    private:

        void updateTrianglesFromIndices();
};

void CreateGrid(SimpleMesh &m, int nY, int nZ, double step);

void CreateBox(SimpleMesh &m, float dx, float dy, float dz);

#endif

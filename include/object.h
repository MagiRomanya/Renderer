#ifndef OBJECT_H_
#define OBJECT_H_

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "external/stb_image.h"

#include <vector>
#include <unordered_map>
#include <string>

#include "mesh.h"
#include "shader.h"
#include <glm/glm.hpp>


class Object{
    public:
        SimpleMesh* mesh;
        Shader shader;

        std::vector<std::string> texture_name;
        std::vector<unsigned int> texture_id;

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        Object() {}
        Object(SimpleMesh &mesh, Shader shader);

        void render();
        void loadTexture(std::string name, std::string path);
        void bindTextures();
};

#endif // OBJECT_H_

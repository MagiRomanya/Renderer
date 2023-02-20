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
#include <glm/gtc/matrix_transform.hpp>


class Object{
    public:
        std::string name;

        SimpleMesh* mesh;
        Shader shader;

        bool render_as_wireframe = false;
        std::vector<std::string> texture_name;
        std::vector<unsigned int> texture_id;

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 translation = glm::vec3(0.0f);
        glm::vec3 scaling = glm::vec3(1.0f);

        Object() {}
        Object(SimpleMesh* mesh, Shader shader);

        inline glm::vec3 model_times_vec3(const glm::vec3 &v) const { return glm::vec3(model * glm::vec4(v, 1)); }

        void updateModelMatrix();

        void render();

        void loadTexture(std::string name, std::string path);

        void bindTextures();
    private:
        static unsigned int num;
};

#endif // OBJECT_H_

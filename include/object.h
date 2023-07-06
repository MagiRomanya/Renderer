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

        bool render_as_wireframe = false;

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        glm::mat4 inverse_model = glm::mat4(1.0f);

        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 translation = glm::vec3(0.0f);
        glm::vec3 scaling = glm::vec3(1.0f);

        Object() {}
        Object(SimpleMesh* mesh);
        Object(SimpleMesh* mesh, Shader* shader);

        inline glm::vec3 toWorld(const glm::vec3 &v) const { return glm::vec3(model * glm::vec4(v, 1)); }

        inline glm::vec3 toLocal(const glm::vec3 &v) const { return glm::vec3(inverse_model * glm::vec4(v, 1)); }

        inline void addShader(Shader* shader) {shaders.push_back(shader); }

        void updateModelMatrix();

        void render();

        void useTexture(const std::string &name, const unsigned int id);

        void bindTextures();

        inline glm::vec3 center() {
            if (!_calculated_center) {
                _center = this->mesh->aproximate_center();
            }
            return _center;
        }

    private:
        std::vector<Shader*> shaders;
        std::vector<std::string> texture_name;
        std::vector<unsigned int> texture_id;

        bool _calculated_center = false;
        glm::vec3 _center;

        static unsigned int num;


};

#endif // OBJECT_H_

#ifndef LITTLE_AXIS_H_
#define LITTLE_AXIS_H_

#include "mesh.h"
#include "shader.h"
#include "object_manager.hpp"
#include "camera.h"

#include "shader_path.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class LittleAxis {
    public:
        LittleAxis(Camera* camera) : camera(camera) {
            manager = ObjectManager();
            manager.loadMesh("axis", MESH_PATH"/3d_axis.obj");
            mesh = manager.getMesh("axis");
            shader = Shader(SHADER_PATH"/axis.vert", SHADER_PATH"/axis.frag");

            projection = glm::perspective(glm::radians(30.0f), 1.0f, 0.1f, 10.0f);
            model = glm::mat4(1.0f);
        }

        void render(){
            shader.use();
            glm::mat4 transform = calculateTransform();
            shader.setMat4("transform", transform);

            glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
        }

    private:
        Shader shader;
        SimpleMesh *mesh;
        ObjectManager manager;
        Camera* camera;

        glm::mat4 projection;
        glm::mat4 model;

        glm::mat4 calculateTransform() {
            glm::mat4 view = glm::lookAt(glm::vec3(0,0,0), camera->Front, camera->Up);

            return projection * view * model;
        }
};

#endif // LITTLE_AXIS_H_

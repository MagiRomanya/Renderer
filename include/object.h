#ifndef OBJECT_H_
#define OBJECT_H_

#include "mesh.h"
#include "shader.h"
#include <glm/glm.hpp>

class Object{
    public:
    SimpleMesh mesh;
    Shader shader;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    Object() {}
    Object(SimpleMesh mesh, Shader shader);

    void render();
};

#endif // OBJECT_H_

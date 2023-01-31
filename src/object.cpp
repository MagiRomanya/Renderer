#include "object.h"

Object::Object(SimpleMesh mesh, Shader shader){
    mesh.CreateVBO();
    this->mesh = mesh;
    this->shader = shader;
}

void Object::render(){
    shader.use();
    mesh.bind();

    // uniforms
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("modelView", view * model);
    shader.setMat4("normalMatrix", glm::inverse(glm::transpose(view * model)));
    shader.setMat4("proj", proj);
    shader.setMat4("modelViewProj", proj * view * model);

    // std::cout << mesh.indices.size() << std::endl;

    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

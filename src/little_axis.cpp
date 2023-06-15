#include "little_axis.hpp"

LittleAxis::LittleAxis(Camera* camera) : camera(camera) {
    mesh = SimpleMesh();
    const char* path = MESH_PATH"/3d_axis.obj";
    mesh.loadFromFile(path);
    shader = Shader(SHADER_PATH"/axis.vert", SHADER_PATH"/axis.frag");
    mesh.createVAO();
}

void LittleAxis::render() {
    shader.use();
    glm::mat4 transform = calculateTransform();
    shader.setMat4("transform", transform);
    mesh.bind();
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

glm::mat4 LittleAxis::calculateTransform() {
    glm::mat4 projection = camera->GetProjMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    const float displacement = 0.75f;
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
    model = glm::scale(model, glm::vec3(0.2f));
    model = glm::rotate(model, -glm::radians(camera->Pitch), glm::vec3(1.0,0,0));
    model = glm::rotate(model, glm::radians(camera->Yaw + 90.0f), camera->WorldUp);
    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(displacement, displacement, 0.0f));
    return model2 * projection * model;
}

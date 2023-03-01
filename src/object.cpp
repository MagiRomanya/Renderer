#include "object.h"

unsigned int Object::num = 0;

Object::Object(SimpleMesh* mesh, Shader* shader){
    num++;
    name = "object_" + std::to_string(num);
    this->mesh = mesh;
    this->shader = shader;
}

void Object::render(){
    shader->use();

    mesh->bind();

    bindTextures();

    // uniforms
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("modelView", view * model);
    shader->setMat4("normalMatrix", glm::inverse(glm::transpose(view * model)));
    shader->setMat4("proj", proj);
    shader->setMat4("modelViewProj", proj * view * model);

    // std::cout << mesh.indices.size() << std::endl;

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the vertex array buffer
    glBindVertexArray(0);
}

void Object::useTexture(const std::string &name, const unsigned int id) {
    texture_id.push_back(id);
    texture_name.push_back(name);
}

void Object::bindTextures(){
    for (unsigned int i = 0; i < texture_id.size(); i++){
        glUniform1i(glGetUniformLocation(shader->program, texture_name[i].c_str()), i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture_id[i]);
    }
}

void Object::updateModelMatrix(){
    glm::vec3 radians = glm::radians(rotation);
    model = glm::mat4(1);

    // Scaling
    model = glm::scale(model, scaling);

    // Translation
    // FIXME: I'm having problems understanding some of the signs regarding translation
    model = glm::translate(model, - translation / scaling); // Component-wise division

    // Rotation
    model = glm::rotate(model, radians.x, glm::vec3(1,0,0));
    model = glm::rotate(model, radians.y, glm::vec3(0,1,0));
    model = glm::rotate(model, radians.z, glm::vec3(0,0,1));

    inverse_model = glm::inverse(model);
}

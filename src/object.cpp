#include "object.h"

Object::Object(SimpleMesh& mesh, Shader shader){
    mesh.createVAO();
    this->mesh = &mesh;
    this->shader = shader;
}

void Object::render(){
    shader.use();
    mesh->bind();

    bindTextures();
    // uniforms
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("modelView", view * model);
    shader.setMat4("normalMatrix", glm::inverse(glm::transpose(view * model)));
    shader.setMat4("proj", proj);
    shader.setMat4("modelViewProj", proj * view * model);

    // std::cout << mesh.indices.size() << std::endl;

    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the vertex array buffer
    glBindVertexArray(0);
}

void Object::loadTexture(std::string name, std::string path){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture_name.push_back(name);
    texture_id.push_back(texture);

    int width, height, nrChanels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChanels, 4);

    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "ERROR: Failed to load the texture: " << path << std::endl;
    }
    stbi_image_free(data);
}

void Object::bindTextures(){
    for (unsigned int i = 0; i < texture_id.size(); i++){
        glUniform1i(glGetUniformLocation(shader.program, texture_name[i].c_str()), i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture_id[i]);
    }
}

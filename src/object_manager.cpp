#include "object_manager.hpp"

ObjectManager::~ObjectManager() {
    // Destroy textures
    size_t n_textures = m_texture.size();
    unsigned int* textures = (unsigned int*) malloc(sizeof(unsigned int) * n_textures);
    size_t count = 0;
    for (auto i : m_texture){
        textures[count++] = i.second;
    }
    glDeleteTextures(n_textures, textures);
    free(textures);

    // Destroy shader programs
    for (auto i : m_shader){
        i.second.destroy();
    }

    // Destroy VAOs
    for (auto i : m_mesh){
        i.second.desrtoyVAO();
    }
}

void ObjectManager::loadTexture(const std::string &name, std::string path){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_texture[name] = texture;

    int width, height, nrChanels;
    // stbi_set_flip_vertically_on_load(true);
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

void ObjectManager::loadShader(const std::string &name, const char* vertexPath, const char* fragmentPath){
    m_shader[name] = Shader(vertexPath, fragmentPath);
}

void ObjectManager::loadMesh(const std::string &name, SimpleMesh mesh){
    m_mesh[name] = SimpleMesh(mesh.vertices, mesh.indices);
}

void ObjectManager::loadMesh(const std::string &name, const std::string &path){
    SimpleMesh m;
    m.loadFromFile(path);
    m_mesh[name] = m;
}

Object ObjectManager::createObject(const std::string &meshName, const std::string &shaderName) {
    if (!m_mesh.contains(meshName)) {
        std::cout << "ERROR::OBJECT_MANAGER::CREATE_OBJECT::Mesh name " << meshName << " not found in object manager" << std::endl;
    }
    if (!m_shader.contains(shaderName)) {
        std::cout << "ERROR::OBJECT_MANAGER::CREATE_OBJECT::Shader name " << shaderName << " not found in object manager" << std::endl;
    }
    Object output(&m_mesh[meshName], &m_shader[shaderName]);
    return output;
}

unsigned int ObjectManager::getTextureID(const std::string &name){
    if (!m_texture.contains(name)){
        std::cout << "ERROR::OBJECT_MANAGER::GET_TEXTURE_ID: Texture name " << name << " not found" << std::endl;
    }
    return m_texture[name];
}

SimpleMesh* ObjectManager::getMesh(const std::string &name){
    if (!m_mesh.contains(name)){
        std::cout << "ERROR::OBJECT_MANAGER::GET_MESH: Mesh name " << name << " not found" << std::endl;
    }
    return &m_mesh[name];
}

Shader* ObjectManager::getShader(const std::string &name){
    if (!m_mesh.contains(name)){
        std::cout << "ERROR::OBJECT_MANAGER::GET_SHADER: Shader name " << name << " not found" << std::endl;
    }
    return &m_shader[name];

}

#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include <vector>
#include <string>
#include <unordered_map>

#include "mesh.h"
#include "shader.h"
#include "object.h"

/* Object manager stores and manages the memory of the VAOs, shader programs and textures.
 * With this memory it can generate objects in a recipie-like manner */
class ObjectManager{
    public:
        ~ObjectManager();

        // Load stuff
        void loadMesh(const std::string &name, SimpleMesh mesh);

        void loadMesh(const std::string &name, const std::string &path);

        void loadShader(const std::string &name, const char* vertexPath, const char* fragmentPath);

        void loadTexture(const std::string &name, std::string path);

        // Create object
        Object createObject(const std::string &meshName, const std::string &shaderName);

        // Utilities
        unsigned int getTextureID(const std::string &name);

        SimpleMesh* getMesh(const std::string &name);

        Shader* getShader(const std::string &name);

    private:
        std::unordered_map<std::string, SimpleMesh> m_mesh;
        std::unordered_map<std::string, Shader> m_shader;
        std::unordered_map<std::string, unsigned int> m_texture;
};

#endif // OBJECT_MANAGER_H_

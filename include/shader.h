#ifndef SHADER_H_
#define SHADER_H_
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
    public:
        unsigned int program;

        Shader() {}
        Shader(const char *vertexPath, const char *fragmentPath);
        ~Shader();

        void use();
        void destroy();

        // Uniforms
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec3(const std::string &name, glm::vec3 value) const;
        void setVec2(const std::string &name, glm::vec2 value) const;
        void setMat4(const std::string &name, glm::mat4 value) const;
};

#endif // SHADER_H_

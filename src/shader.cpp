#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath){
    /* Loads a vertex shader and a fragment shader, compiles it and links it into
     * a shader program */

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vShaderCode = read_shader_soruce(vertexPath);
    std::string fShaderCode = read_shader_soruce(fragmentPath);

    // 2. compile shaders
    unsigned int vertex = compile_shader(vShaderCode, GL_VERTEX_SHADER);
    unsigned int fragment = compile_shader(fShaderCode, GL_FRAGMENT_SHADER);

    // Shader program
    int success;
    char infoLog[512];
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    // print linking errors if any
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(const char *vertexPath, const char *geometryPath, const char* fragmentPath){
    /* Loads a vertex shader and a fragment shader, compiles it and links it into
     * a shader program */
    std::string vShaderCode = read_shader_soruce(vertexPath);
    std::string gShaderCode = read_shader_soruce(geometryPath);
    std::string fShaderCode = read_shader_soruce(fragmentPath);

    // 2. compile shaders
    unsigned int vertex = compile_shader(vShaderCode, GL_VERTEX_SHADER);
    unsigned int geometry = compile_shader(gShaderCode, GL_GEOMETRY_SHADER);
    unsigned int fragment = compile_shader(fShaderCode, GL_FRAGMENT_SHADER);
    // Shader program
    int success;
    char infoLog[512];
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, geometry);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    // print linking errors if any
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
}

std::string Shader::read_shader_soruce(const char* path){
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        shaderFile.open(path);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        shaderCode = shaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ "<< path << std::endl;
    }
    return shaderCode;
}

unsigned int Shader::compile_shader(const std::string source, GLenum shaderType){
    unsigned int shader;
    int success;
    char infoLog[512];
    const char* source_code = source.c_str();
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source_code, NULL);
    glCompileShader(shader);

    // print compile errors if any
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::string shaderTypeString= "UNKNOWN";
        switch(shaderType) {
            case GL_VERTEX_SHADER:
                shaderTypeString = "VERTEX";
                break;
            case GL_FRAGMENT_SHADER:
                shaderTypeString = "FRAGMENT";
                break;
            case GL_GEOMETRY_SHADER:
                shaderTypeString = "GEOMETRY";
                break;
        }
        std::cerr << "ERROR::SHADER::"<< shaderTypeString<<"::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    };
    return shader;
}

Shader::~Shader(){
    // glDeleteProgram(program);
}

void Shader::use(){
    glUseProgram(program);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int) value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const{
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, glm::vec2 value) const{
    glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const{
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::destroy() {
    glDeleteProgram(program);
}

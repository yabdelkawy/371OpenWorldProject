#ifndef SHADER_H
#define SHADER_H

#define GLEW_STATIC 1 // necessary for glew

#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    // constructor reads and builds the shader
    explicit Shader(const char* shaderName);

    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr);

    // use/activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    void setVec3(const std::string &name, float x, float y, float z) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

private:
    void createShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    static void checkCompileErrors(GLuint shader_program, const std::string &type);
};

#endif //SHADER_H

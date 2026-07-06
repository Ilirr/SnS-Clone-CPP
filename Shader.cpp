#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	: ID(0)
{
    std::ifstream vFile(vertexPath);
    std::ifstream fFile(fragmentPath);
    if (!vFile || !fFile) {
        throw std::runtime_error("Failed to open shader files: " + vertexPath + " or " + fragmentPath);
    }

    std::stringstream vSs, fSs;
    vSs << vFile.rdbuf();
    fSs << fFile.rdbuf();
    std::string vCode = vSs.str();
    std::string fCode = fSs.str();
    const char* vSrc = vCode.c_str();
    const char* fSrc = fCode.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vSrc, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fSrc, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

}
Shader::~Shader()
{
    if (ID)
        glDeleteProgram(ID);
}
void Shader::use() const
{
    if (ID)
        glUseProgram(ID);

}
void Shader::vec4(const std::string& name, const glm::vec4& v) const
{
    if (!ID) return;
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUseProgram(ID);
    glUniform4fv(loc, 1, glm::value_ptr(v));

}
void Shader::mat4(const std::string& name, const glm::mat4& mat) const
{
    if (!ID) return;
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1) return;
    glUseProgram(ID);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::checkCompileErrors(GLuint shader, const std::string& type)
{
    GLint success = 0;
    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logLen = 0;
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLen);
            std::string info(logLen, ' ');
            glGetProgramInfoLog(shader, logLen, nullptr, info.data());
            throw std::runtime_error("Program link error: " + info);
        }
    }
    else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
            std::string info(logLen, ' ');
            glGetShaderInfoLog(shader, logLen, nullptr, info.data());
            throw std::runtime_error(type + " shader compile error: " + info);
        }
    }
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
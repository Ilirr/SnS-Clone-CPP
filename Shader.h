#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	void use() const;

	void mat4(const std::string& name, const glm::mat4& mat) const;
	void vec4(const std::string& name, const glm::vec4& v) const;

	void setInt(const std::string& name, int value) const;

private:
	GLuint ID = 0;
	void checkCompileErrors(GLuint shader, const std::string& type);

};
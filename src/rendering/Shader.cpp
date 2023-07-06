#include "Shader.hpp"
#include "GL/glew.h"
#include "ShaderCompiler.hpp"

#include "../util/ErrorHandling.hpp"

Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
	m_ShaderID = shadercompiler::loadShader(vertexPath, fragmentPath);
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_ShaderID));
}

Shader::Shader(Shader&& other) noexcept {
	*this = std::move(other);
}

Shader& Shader::operator=(Shader&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	m_ShaderID = other.m_ShaderID;
	other.m_ShaderID = 0;

	return *this;
}

void Shader::setLayout(VertexLayout& layout) {
	m_Layout = std::move(layout);
}

void Shader::bind() const {
	GLCall(glUseProgram(m_ShaderID));
}

void Shader::unbind() const {
	GLCall(glUseProgram(0));
}
	
int Shader::getUniformLocation(std::string_view uniformName) { 
	bind();

	GLCall(int location = glGetUniformLocation(m_ShaderID, uniformName.data()));

	return location;
}

//UNIFORM SETTERS

void Shader::setUniform1i(std::string_view uniformName, const int value) {
	GLCall(glUniform1i(getUniformLocation(uniformName), value));
}

void Shader::setUniform1f(std::string_view uniformName, const float value) {
	GLCall(glUniform1f(getUniformLocation(uniformName), value));
}

void Shader::setUniform3fv(std::string_view uniformName, const glm::vec3 value) {
	GLCall(glUniform3fv(getUniformLocation(uniformName), 1, &value[0]));
}

void Shader::setUniformMat3f(std::string_view uniformName, const glm::mat3& value) {
	glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniformMat4f(std::string_view uniformName, const glm::mat4& value) {
	glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, &value[0][0]);
}
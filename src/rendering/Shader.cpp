#include "Shader.hpp"
#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include "../util/ErrorHandling.hpp"

Shader::Shader(std::wstring_view vertexPath, std::wstring_view fragmentPath) {
	GLCall(m_ShaderID = glCreateProgram());
	unsigned int vertexShader = 0, fragmentShader = 0;

	try {
		vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
		fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	GLCall(glAttachShader(m_ShaderID, vertexShader));
	GLCall(glAttachShader(m_ShaderID, fragmentShader));
	GLCall(glLinkProgram(m_ShaderID));
	GLCall(glValidateProgram(m_ShaderID));

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));
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

void Shader::bind() const {
	GLCall(glUseProgram(m_ShaderID));
}

void Shader::unbind() const {
	GLCall(glUseProgram(0));
}

unsigned int Shader::compileShader(const std::filesystem::path& shaderPath, unsigned int shaderType) const {
	if (!std::filesystem::is_regular_file(shaderPath)) {
		throw std::runtime_error("Shader Path Not found or not a regular file! Path: " + shaderPath.string());
	}

	std::string sourceString = getShaderSource(shaderPath);
	const char* sourceCString = sourceString.c_str();

	GLCall(unsigned int shaderID = glCreateShader(shaderType));

	GLCall(glShaderSource(shaderID, 1, &sourceCString, NULL));
	GLCall(glCompileShader(shaderID));

	checkCompilationStatus(shaderID);

	return shaderID;
}

std::string Shader::getShaderSource(const std::filesystem::path& shaderPath) const {
	std::ifstream fileStream(shaderPath);
	if (!fileStream) {
		throw std::runtime_error("Failed to open shader file: " + shaderPath.string());
	}
	return std::string((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
}

void Shader::checkCompilationStatus(unsigned int shader) const {
	int result;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		char message[256];
		GLCall(glGetShaderInfoLog(shader, length, &length, message));
		GLCall(glDeleteShader(shader));

		throw std::runtime_error("Shader Compilation Error: " + std::string(message));
	}
}
	
int Shader::getUniformLocation(std::string_view uniformName) { 
	bind();
	if (m_UniformCache.contains(std::string(uniformName))) {
		return m_UniformCache.at(std::string(uniformName));
	}

	GLCall(int location = glGetUniformLocation(m_ShaderID, uniformName.data()));

	if (location == -1) {
		std::cout << "Warning: Uniform " << uniformName << " doesn't exist!" << std::endl;
	}

	m_UniformCache[std::string(uniformName)] = location;
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

void Shader::setUniformMat4f(std::string_view uniformName, const glm::mat4& value) {
	glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, &value[0][0]);
}
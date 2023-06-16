#include "Shader.hpp"
#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include "../util/ErrorHandling.hpp"

Shader::Shader(std::wstring_view a_VertexPath, std::wstring_view a_FragmentPath) {
	GLCall(m_ShaderID = glCreateProgram());
	unsigned int vertexShader = 0, fragmentShader = 0;

	try {
		vertexShader = compileShader(a_VertexPath, GL_VERTEX_SHADER);
		fragmentShader = compileShader(a_FragmentPath, GL_FRAGMENT_SHADER);
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

unsigned int Shader::compileShader(const std::filesystem::path& a_ShaderPath, unsigned int a_ShaderType) const {
	if (!std::filesystem::is_regular_file(a_ShaderPath)) {
		throw std::runtime_error("Shader Path Not found or not a regular file! Path: " + a_ShaderPath.string());
	}

	std::string sourceString = getShaderSource(a_ShaderPath);
	const char* sourceCString = sourceString.c_str();

	GLCall(unsigned int shaderID = glCreateShader(a_ShaderType));

	GLCall(glShaderSource(shaderID, 1, &sourceCString, NULL));
	GLCall(glCompileShader(shaderID));

	checkCompilationStatus(shaderID);

	return shaderID;
}

std::string Shader::getShaderSource(const std::filesystem::path& a_ShaderPath) const {
	std::ifstream fileStream(a_ShaderPath);
	if (!fileStream) {
		throw std::runtime_error("Failed to open shader file: " + a_ShaderPath.string());
	}
	return std::string((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
}

void Shader::checkCompilationStatus(unsigned int a_Shader) const {
	int result;
	GLCall(glGetShaderiv(a_Shader, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(a_Shader, GL_INFO_LOG_LENGTH, &length));
		char message[256];
		GLCall(glGetShaderInfoLog(a_Shader, length, &length, message));
		GLCall(glDeleteShader(a_Shader));

		throw std::runtime_error("Shader Compilation Error: " + std::string(message));
	}
}
	
int Shader::getUniformLocation(std::string_view a_UniformName) { 
	bind();
	if (m_UniformCache.find(std::string(a_UniformName)) != m_UniformCache.end()) {
		return m_UniformCache.at(std::string(a_UniformName));
	}

	GLCall(int location = glGetUniformLocation(m_ShaderID, a_UniformName.data()));

	if (location == -1) {
		std::cout << "Warning: Uniform " << a_UniformName << " doesn't exist!" << std::endl;
	}

	m_UniformCache[std::string(a_UniformName)] = location;
	return location;
}

//UNIFORM SETTERS

void Shader::setUniform1i(std::string_view a_UniformName, const int a_Value) {
	GLCall(glUniform1i(getUniformLocation(a_UniformName), a_Value));
}

void Shader::setUniform1f(std::string_view a_UniformName, const float a_Value) {
	GLCall(glUniform1f(getUniformLocation(a_UniformName), a_Value));
}

void Shader::setUniform3fv(std::string_view a_UniformName, const glm::vec3 a_Value) {
	GLCall(glUniform3fv(getUniformLocation(a_UniformName), 1, &a_Value[0]));
}

void Shader::setUniformMat4f(std::string_view a_UniformName, const glm::mat4& a_Value) {
	glUniformMatrix4fv(getUniformLocation(a_UniformName), 1, GL_FALSE, &a_Value[0][0]);
}
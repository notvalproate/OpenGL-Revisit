#include "Shader.hpp"
#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include <vector>

Shader::Shader(const std::filesystem::path& p_VertexPath, const std::filesystem::path& p_FragmentPath) {
	m_ShaderID = glCreateProgram();
	unsigned int t_VertexShader = 0, t_FragmentShader = 0;

	try {
		t_VertexShader = CompileShader(p_VertexPath, GL_VERTEX_SHADER);
		t_FragmentShader = CompileShader(p_FragmentPath, GL_FRAGMENT_SHADER);
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	glAttachShader(m_ShaderID, t_VertexShader);
	glAttachShader(m_ShaderID, t_FragmentShader);
	glLinkProgram(m_ShaderID);
	glValidateProgram(m_ShaderID);

	glDeleteShader(t_VertexShader);
	glDeleteShader(t_FragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(m_ShaderID);
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

void Shader::Bind() const {
	glUseProgram(m_ShaderID);
}

void Shader::Unbind() const {
	glUseProgram(0);
}

unsigned int Shader::CompileShader(const std::filesystem::path& p_ShaderPath, unsigned int p_ShaderType) const {
	if (!std::filesystem::is_regular_file(p_ShaderPath)) {
		throw std::runtime_error("Shader Path Not found or not a regular file! Path: " + p_ShaderPath.string());
	}

	std::string t_Src = GetShaderSrc(p_ShaderPath);
	const char* t_ShaderSrc = t_Src.c_str();
	unsigned int t_Shader = glCreateShader(p_ShaderType);

	glShaderSource(t_Shader, 1, &t_ShaderSrc, NULL);
	glCompileShader(t_Shader);

	CheckCompilationStatus(t_Shader);

	return t_Shader;
}

std::string Shader::GetShaderSrc(const std::filesystem::path& p_ShaderPath) const {
	std::ifstream t_File(p_ShaderPath);
	if (!t_File) {
		throw std::runtime_error("Failed to open shader file: " + p_ShaderPath.string());
	}
	return std::string((std::istreambuf_iterator<char>(t_File)), std::istreambuf_iterator<char>());
}

void Shader::CheckCompilationStatus(unsigned int p_Shader) const {
	int result;
	glGetShaderiv(p_Shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(p_Shader, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> message(length);
		glGetShaderInfoLog(p_Shader, length, &length, message.data());
		glDeleteShader(p_Shader);

		throw std::runtime_error("Shader Compilation Error: " + std::string(message.data()));
	}
}
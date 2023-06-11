#include "Shader.hpp"
#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "../util/ErrorHandling.hpp"

Shader::Shader(std::wstring_view p_VertexPath, std::wstring_view p_FragmentPath) {
	GLCall(m_ShaderID = glCreateProgram());
	unsigned int t_VertexShader = 0, t_FragmentShader = 0;

	try {
		t_VertexShader = CompileShader(p_VertexPath, GL_VERTEX_SHADER);
		t_FragmentShader = CompileShader(p_FragmentPath, GL_FRAGMENT_SHADER);
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	GLCall(glAttachShader(m_ShaderID, t_VertexShader));
	GLCall(glAttachShader(m_ShaderID, t_FragmentShader));
	GLCall(glLinkProgram(m_ShaderID));
	GLCall(glValidateProgram(m_ShaderID));

	GLCall(glDeleteShader(t_VertexShader));
	GLCall(glDeleteShader(t_FragmentShader));
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

void Shader::Bind() const {
	GLCall(glUseProgram(m_ShaderID));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

unsigned int Shader::CompileShader(const std::filesystem::path& p_ShaderPath, unsigned int p_ShaderType) const {
	if (!std::filesystem::is_regular_file(p_ShaderPath)) {
		throw std::runtime_error("Shader Path Not found or not a regular file! Path: " + p_ShaderPath.string());
	}

	std::string t_Src = GetShaderSrc(p_ShaderPath);
	const char* t_ShaderSrc = t_Src.c_str();
	GLCall(unsigned int t_Shader = glCreateShader(p_ShaderType));

	GLCall(glShaderSource(t_Shader, 1, &t_ShaderSrc, NULL));
	GLCall(glCompileShader(t_Shader));

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
	GLCall(glGetShaderiv(p_Shader, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(p_Shader, GL_INFO_LOG_LENGTH, &length));
		std::vector<char> message(length);
		GLCall(glGetShaderInfoLog(p_Shader, length, &length, message.data()));
		GLCall(glDeleteShader(p_Shader));

		throw std::runtime_error("Shader Compilation Error: " + std::string(message.data()));
	}
}
	
int Shader::GetUniformLocation(const std::string& p_UniformName) {
	if (m_UniformCache.find(p_UniformName) != m_UniformCache.end()) {
		std::cout << "Got " << p_UniformName << " from cache!";
		return m_UniformCache[p_UniformName];
	}

	Bind();
	GLCall(int t_Location = glGetUniformLocation(m_ShaderID, p_UniformName.data()));

	if (t_Location == -1) {
		std::cout << "Warning: Uniform " << p_UniformName << " doesn't exist!" << std::endl;
	}

	m_UniformCache[p_UniformName] = t_Location;
	return t_Location;
}

//UNIFORM SETTERS

void Shader::SetUniform1i(const std::string& p_UniformName, const int p_Value) {
	GLCall(glUniform1i(GetUniformLocation(p_UniformName), p_Value));
}
#include "Shader.hpp"
#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include <vector>

Shader::Shader(const std::string& p_VertexPath, const std::string& p_FragmentPath) {
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

unsigned int Shader::CompileShader(const std::string& p_ShaderPath, const unsigned int& p_ShaderType) const {
	std::ifstream t_File(p_ShaderPath.c_str());
	std::string t_ShaderSrc((std::istreambuf_iterator<char>(t_File)), std::istreambuf_iterator<char>());

	unsigned int t_Shader = glCreateShader(p_ShaderType);
	const char* t_Src = t_ShaderSrc.c_str();
	glShaderSource(t_Shader, 1, &t_Src, NULL);
	glCompileShader(t_Shader);

	int result;
	glGetShaderiv(t_Shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(t_Shader, GL_INFO_LOG_LENGTH, &length);  
		std::vector<char> message(length);
		glGetShaderInfoLog(t_Shader, length, &length, message.data());
		glDeleteShader(t_Shader);
		
		throw std::runtime_error("Shader Compilation Error: " + std::string(message.data()));
	}

	return t_Shader;
}

void Shader::Bind() const {
	glUseProgram(m_ShaderID);
}

void Shader::Unbind() const {
	glUseProgram(0);
}
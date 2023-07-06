#include "ShaderCompiler.hpp"
#include "../util/ErrorHandling.hpp"

unsigned int shadercompiler::loadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
	GLCall(unsigned int shaderID = glCreateProgram());
	unsigned int vertexShader = 0, fragmentShader = 0;

	try {
		vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
		fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	GLCall(glAttachShader(shaderID, vertexShader));
	GLCall(glAttachShader(shaderID, fragmentShader));
	GLCall(glLinkProgram(shaderID));
	GLCall(glValidateProgram(shaderID));

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));

	return shaderID;
}

unsigned int shadercompiler::compileShader(const std::filesystem::path& shaderPath, unsigned int shaderType) {
	if (!std::filesystem::is_regular_file(shaderPath)) {
		throw std::runtime_error("Shader Path Not found or not a regular file! Path: " + shaderPath.string());
	}

	std::string sourceString = std::move(getShaderSource(shaderPath));
	const char* sourceCString = sourceString.c_str();

	GLCall(unsigned int shaderID = glCreateShader(shaderType));

	GLCall(glShaderSource(shaderID, 1, &sourceCString, NULL));
	GLCall(glCompileShader(shaderID));

	checkCompilationStatus(shaderID);

	return shaderID;
}

std::string shadercompiler::getShaderSource(const std::filesystem::path& shaderPath) {
	std::ifstream fileStream(shaderPath);
	if (!fileStream) {
		throw std::runtime_error("Failed to open shader file: " + shaderPath.string());
	}
	return std::string((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
}

void shadercompiler::checkCompilationStatus(unsigned int shader) {
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
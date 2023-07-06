#include "GL/glew.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace shadercompiler {
	unsigned int loadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	unsigned int compileShader(const std::filesystem::path& shaderPath, unsigned int shaderType);
	std::string getShaderSource(const std::filesystem::path& shaderPath);
	void checkCompilationStatus(unsigned int Shader);
}
#pragma once
#include <filesystem>
#include <unordered_map>
#include <span>
#include "glm/glm.hpp"

class Shader {
public:
	Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	~Shader();

	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	void bind() const;
	void unbind() const;

	[[nodiscard]] unsigned int getID() const { return m_ShaderID; }
	[[nodiscard]] int getUniformLocation(std::string_view UniformName);

	void setUniform1i(std::string_view uniformName, const int value);
	void setUniform1f(std::string_view uniformName, const float value);
	void setUniform3fv(std::string_view uniformName, const glm::vec3 value);
	void setUniformMat4f(std::string_view uniformName, const glm::mat4& value);
private:
	unsigned int m_ShaderID;
	std::unordered_map<std::string, int> m_UniformCache;

	unsigned int compileShader(const std::filesystem::path& shaderPath, unsigned int shaderType) const;
	std::string getShaderSource(const std::filesystem::path& shaderPath) const;
	void checkCompilationStatus(unsigned int Shader) const;
};
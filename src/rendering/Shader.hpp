#pragma once
#include <string>
#include <filesystem>
#include <unordered_map>
#include <span>
#include "glm/glm.hpp"

class Shader {
public:
	Shader(std::wstring_view a_VertexPath, std::wstring_view a_FragmentPath);
	~Shader();

	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	void bind() const;
	void unbind() const;

	[[nodiscard]] unsigned int getID() const { return m_ShaderID; }
	[[nodiscard]] int getUniformLocation(std::string_view a_UniformName);

	void setUniform1i(std::string_view a_UniformName, const int a_Value);
	void setUniform1f(std::string_view a_UniformName, const float a_Value);
	void setUniform3fv(std::string_view a_UniformName, const glm::vec3 a_Value);
	void setUniformMat4f(std::string_view a_UniformName, const glm::mat4& a_Value);
private:
	unsigned int m_ShaderID;
	std::unordered_map<std::string, int> m_UniformCache;

	unsigned int compileShader(const std::filesystem::path& a_ShaderPath, unsigned int a_ShaderType) const;
	std::string getShaderSource(const std::filesystem::path& a_ShaderPath) const;
	void checkCompilationStatus(unsigned int a_Shader) const;
};
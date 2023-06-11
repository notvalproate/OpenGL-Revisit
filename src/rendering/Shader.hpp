#pragma once
#include <string>
#include <filesystem>
#include <unordered_map>

class Shader {
public:
	Shader(std::wstring_view p_VertexPath, std::wstring_view p_FragmentPath);
	~Shader();

	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	void Bind() const;
	void Unbind() const;

	[[nodiscard]] unsigned int GetID() const { return m_ShaderID; }
	[[nodiscard]] int GetUniformLocation(std::string_view p_UniformName);

	void SetUniform1i(std::string_view p_UniformName, const int p_Value);
private:
	unsigned int m_ShaderID;
	std::unordered_map<std::string, int> m_UniformCache;

	unsigned int CompileShader(const std::filesystem::path& p_ShaderPath, unsigned int p_ShaderType) const;
	std::string GetShaderSrc(const std::filesystem::path& p_ShaderPath) const;
	void CheckCompilationStatus(unsigned int p_Shader) const;
};
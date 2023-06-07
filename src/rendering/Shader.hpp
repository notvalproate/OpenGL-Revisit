#pragma once
#include <string>
#include <filesystem>

class Shader {
public:
	Shader(const std::filesystem::path& p_VertexPath, const std::filesystem::path& p_FragmentPath);
	~Shader();

	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	void Bind() const;
	void Unbind() const;

	[[nodiscard]] unsigned int GetID() const { return m_ShaderID; }
private:
	unsigned int m_ShaderID;

	unsigned int CompileShader(const std::filesystem::path& p_ShaderPath, const unsigned int& p_ShaderType) const;
	std::string GetShaderSrc(const std::filesystem::path& p_ShaderPath) const;
	void CheckCompilationStatus(const unsigned int& p_Shader) const;
};
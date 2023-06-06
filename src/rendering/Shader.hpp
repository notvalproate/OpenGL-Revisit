#pragma once
#include <string>

class Shader {
public:
	Shader() = delete;
	Shader(const std::string& p_VertexPath, const std::string& p_FragmentPath);
	~Shader();

	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	Shader& operator=(Shader&& other) noexcept {
		if (this == &other) {
			return *this;
		}

		m_ShaderID = other.m_ShaderID;
		other.m_ShaderID = 0;

		return *this;
	}

	Shader(Shader&& other) noexcept {
		*this = std::move(other);
	}

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_ShaderID;

	unsigned int CompileShader(const std::string& p_ShaderPath, const unsigned int& p_ShaderType) const;
};
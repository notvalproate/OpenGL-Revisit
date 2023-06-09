#pragma once
#include "glm/glm.hpp"
#include <filesystem>

#include "VertexLayout.hpp"

class Shader {
public:
	Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	~Shader();

	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	void setLayout(VertexLayout& layout);

	void bind() const;
	void unbind() const;

	[[nodiscard]] const VertexLayout& getLayout() const { return m_Layout; }
	[[nodiscard]] int getUniformLocation(std::string_view UniformName);

	void setUniform1i(std::string_view uniformName, const int value);
	void setUniform1f(std::string_view uniformName, const float value);
	void setUniform3fv(std::string_view uniformName, const glm::vec3 value);
	void setUniformMat3f(std::string_view uniformName, const glm::mat3& value);
	void setUniformMat4f(std::string_view uniformName, const glm::mat4& value);
private:
	unsigned int m_ShaderID;
	VertexLayout m_Layout;
};
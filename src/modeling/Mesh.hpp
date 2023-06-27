#pragma once
#include <glm/glm.hpp>
#include "../textures/Texture2D.hpp"
#include "../rendering/VertexArray.hpp"
#include "../rendering/Buffers.hpp"
#include "../rendering/Shader.hpp"

#include <span>
#include <array>

class Mesh {
public:
	Mesh(const std::span<float>& vertices, const std::span<unsigned int>& indices, std::vector<Texture2D>& textures, Shader* shader);
	~Mesh();

	Mesh(const Mesh& other) = delete;
	Mesh& operator=(const Mesh& other) = delete;

	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	void draw() const;
private:
	VertexArray* m_VAO;
	IndexBuffer* m_IBO;
	Shader* m_Shader;
	std::vector<Texture2D> m_Textures;

	void bindTextures() const;
};
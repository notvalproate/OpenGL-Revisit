#pragma once
#include <glm/glm.hpp>
#include "../textures/Texture2D.hpp"
#include "../textures/Material.hpp"
#include "../rendering/VertexArray.hpp"
#include "../rendering/Buffers.hpp"
#include "../rendering/Shader.hpp"

#include <span>
#include <array>
#include <memory>

class Mesh {
public:
	Mesh(const std::span<float>& vertices, const std::span<unsigned int>& indices, std::shared_ptr<Material> material, Shader* shader);
	~Mesh() = default;

	Mesh(const Mesh& other) = delete;
	Mesh& operator=(const Mesh& other) = delete;

	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	void draw() const;
private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IBO;
	std::shared_ptr<Material> m_Material;
	Shader* m_Shader;
};
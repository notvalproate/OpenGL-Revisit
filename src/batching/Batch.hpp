#pragma once
#include "../rendering/VertexArray.hpp"
#include "../textures/Material.hpp"

#include <vector>

class Batch {
public:
	Batch() = default;

	void add(std::vector<float>& vertices, std::vector<unsigned int>& indices);
	void finalize(std::vector<Material>& materials);

	void draw(Shader* shader);
private:
	std::vector<Mesh> m_Meshes;

	std::vector<float> m_Vertices{};
	std::vector<unsigned int> m_Indices{};

	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IBO;
	std::vector<Material> m_Materials{};
};
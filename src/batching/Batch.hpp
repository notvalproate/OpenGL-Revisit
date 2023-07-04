#pragma once
#include "../rendering/VertexArray.hpp"
#include "../textures/Material.hpp"
#include "../modeling/Mesh.hpp"

#include <vector>

struct BatchInfo {
	std::size_t numOfVertices{};
	std::size_t numOfIndices{};
};

class Batch {
public:
	Batch() = default;

	void add(Mesh& mesh);
	void finalize(Shader* shader);

	void draw() const;
private:
	std::vector<Mesh> m_Meshes{};

	BatchInfo m_BatchInfo{};

	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IBO;

	void parseMeshes(std::vector<float>& finalVertices, std::vector<unsigned int>& finalIndices, std::size_t stride);
};
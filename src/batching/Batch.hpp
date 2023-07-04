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
	BatchInfo m_BatchInfo{};

	std::vector<Mesh> m_Meshes{};

	VertexBuffer* m_VBO;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IBO;
};
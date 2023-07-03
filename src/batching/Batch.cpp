#include "Batch.hpp"

void Batch::add(Mesh& mesh) {
	m_Meshes.push_back(std::move(mesh));
}

void Batch::finalize(Shader* shader) {
	std::vector<float> finalVertices;
	std::vector<unsigned int> finalIndices;

	for (const auto& mesh : m_Meshes) {
		std::size_t lastIndex = finalVertices.size() / 9;

		std::copy(mesh.m_Vertices.begin(), mesh.m_Vertices.end(), std::back_inserter(finalVertices));

		std::vector<unsigned int> transformedIndices = mesh.m_Indices;
		std::transform(
			transformedIndices.begin(),
			transformedIndices.end(),
			transformedIndices.begin(),
			[lastIndex](int value) {
				return value + lastIndex;
			}
		);

		finalIndices.insert(
			finalIndices.begin(),
			std::make_move_iterator(transformedIndices.begin()),
			std::make_move_iterator(transformedIndices.end())
		);
	}

	m_VBO = new VertexBuffer(finalVertices);
	m_VAO = std::make_unique<VertexArray>(*m_VBO, shader->getLayout());
	m_IBO = std::make_unique<IndexBuffer>(finalIndices);
}

void Batch::draw() const {
	m_VAO->bind();
	m_IBO->bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_IBO->getCount(), GL_UNSIGNED_INT, nullptr));
}
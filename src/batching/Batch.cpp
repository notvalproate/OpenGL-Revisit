#include "Batch.hpp"

void Batch::add(Mesh& mesh) {
	m_BatchInfo.numOfIndices += mesh.m_Indices.size();
	m_BatchInfo.numOfVertices += mesh.m_Vertices.size();
	m_Meshes.push_back(std::move(mesh));
}

void Batch::finalize(Shader* shader) {
	std::vector<float> finalVertices;
	finalVertices.reserve(m_BatchInfo.numOfVertices);
	std::vector<unsigned int> finalIndices;
	finalIndices.reserve(m_BatchInfo.numOfIndices);

	parseMeshes(finalVertices, finalIndices, shader->getLayout().getStride());

	m_VBO = std::make_unique<VertexBuffer>(finalVertices);
	m_VAO = std::make_unique<VertexArray>(m_VBO.get(), shader->getLayout());
	m_IBO = std::make_unique<IndexBuffer>(finalIndices);
}

void Batch::parseMeshes(std::vector<float>& finalVertices, std::vector<unsigned int>& finalIndices, std::size_t stride) {
	for (auto& mesh : m_Meshes) {
		std::size_t lastIndex = finalVertices.size() / stride;

		finalVertices.insert(finalVertices.end(), mesh.m_Vertices.begin(), mesh.m_Vertices.end());

		std::transform(
			mesh.m_Indices.begin(),
			mesh.m_Indices.end(),
			mesh.m_Indices.begin(),
			[&lastIndex](int value) {
				return value + lastIndex;
			}
		);

		finalIndices.insert(finalIndices.end(), mesh.m_Indices.begin(), mesh.m_Indices.end());
	}
}

void Batch::draw() const {
	m_VAO->bind();
	m_IBO->bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_IBO->getCount(), GL_UNSIGNED_INT, nullptr));
}
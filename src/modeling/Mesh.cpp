#include "Mesh.hpp"
#include <iostream>

Mesh::Mesh(const std::span<float>& vertices, const std::span<unsigned int>& indices, std::shared_ptr<Material> material, Shader* shader)
	: m_Shader(shader), m_Material(material) {

	VertexBuffer vbo(vertices);

	try {
		m_VAO = std::make_unique<VertexArray>(vbo, m_Shader->getLayout());
		m_IBO = std::make_unique<IndexBuffer>(indices);
	}
	catch (const std::bad_alloc& e) {
		std::cerr << "Allocation for mesh failed! Exception thrown: " << e.what() << std::endl;
	}
}

Mesh::Mesh(Mesh&& other) noexcept {
	*this = std::move(other);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	m_VAO = std::move(other.m_VAO);
	m_IBO = std::move(other.m_IBO);
	m_Shader = other.m_Shader;
	m_Material = other.m_Material;

	return *this;
}

void Mesh::draw() const {
	m_Material->bind(m_Shader);
	m_VAO->bind();
	m_IBO->bind();
	m_Shader->bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_IBO->getCount(), GL_UNSIGNED_INT, nullptr));
	m_Material->unbind(m_Shader);
}
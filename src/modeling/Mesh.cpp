#include "Mesh.hpp"
#include <iostream>

Mesh::Mesh(const std::span<float>& vertices, const std::span<unsigned int>& indices, std::vector<Texture2D>& textures, Shader* shader) 
	: m_Shader(shader), m_Textures(std::move(textures)), m_Model(glm::mat4(1.0f)) {

	VertexBuffer vbo(vertices);

	try {
		m_VAO = new VertexArray(vbo, m_Shader->getLayout());
		m_IBO = new IndexBuffer(indices);
	}
	catch (const std::bad_alloc& e) {
		std::cerr << "Allocation for mesh failed! Exception thrown: " << e.what() << std::endl;
	}
}

Mesh::~Mesh() {
	delete m_VAO;
	delete m_IBO;
}

Mesh::Mesh(Mesh&& other) noexcept {
	*this = std::move(other);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	m_VAO = other.m_VAO;
	m_IBO = other.m_IBO;
	m_Shader = other.m_Shader;
	m_Model = std::move(other.m_Model);
	m_Textures = std::move(other.m_Textures);

	other.m_VAO = nullptr;
	other.m_IBO = nullptr;
	other.m_Shader = nullptr;

	return *this;
}

void Mesh::setModelMatrix(const glm::mat4& model) { 
	m_Model = model;
}

void Mesh::draw() const {
	bindTextures();
	m_Shader->setUniformMat4f("u_Model", m_Model);

	m_VAO->bind();
	m_IBO->bind();
	m_Shader->bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_IBO->getCount(), GL_UNSIGNED_INT, nullptr));
}

void Mesh::bindTextures() const {
	unsigned short diffuseMapCount = 0, specularMapCount = 0;

	for (std::size_t i = 0; i < m_Textures.size(); i++) {
		TextureType type = m_Textures[i].getType();

		if (type == TextureType::DIFFUSE) {
			m_Shader->setUniform1i("u_Material.diffusion[" + std::to_string(diffuseMapCount++) + "]", i);
		}
		else if (type == TextureType::SPECULAR) {
			m_Shader->setUniform1i("u_Material.specular[" + std::to_string(specularMapCount++) + "]", i);
		}

		m_Textures[i].bind(i);
	}
	m_Shader->setUniform1f("u_Material.shininess", 32.0f);
}
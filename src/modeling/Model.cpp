#include "Model.hpp"
#include <iostream>

Model::Model(std::vector<std::unique_ptr<Mesh>>& meshes, std::vector<Material>& materials, Shader* shader) :
	m_Meshes(std::move(meshes)),
	m_Materials(std::move(materials)),
	m_Shader(shader),
	m_ModelMatrix(glm::mat4(1.0f)),
	m_NormalMatrix(glm::transpose(glm::inverse(m_ModelMatrix))) { }

Model::Model(Model&& other) noexcept {
	*this = std::move(other);
}

Model& Model::operator=(Model&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	m_Meshes = std::move(other.m_Meshes);
	m_Materials = std::move(other.m_Materials);
	m_ModelMatrix = std::move(other.m_ModelMatrix);
	m_NormalMatrix = std::move(other.m_NormalMatrix);
	m_Shader = other.m_Shader;
}

void Model::draw() const {
	m_Shader->bind();
	m_Shader->setUniformMat3f("u_NormalMatrix", m_NormalMatrix);
	m_Shader->setUniformMat4f("u_Model", m_ModelMatrix);

	std::size_t textureCount = 0;

	for (const auto& material : m_Materials) {
		material.bind(m_Shader, textureCount);
	}

	for (const auto& mesh : m_Meshes) {
		mesh.get()->draw();
	}
}

void Model::setModelMatrix(const glm::mat4& model) {
	m_ModelMatrix = model;
	m_NormalMatrix = glm::transpose(glm::inverse(m_ModelMatrix));
}
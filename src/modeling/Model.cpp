#include "Model.hpp"
#include <iostream>

Model::Model(std::vector<Mesh>& meshes, Shader* shader) : 
	m_Meshes(std::move(meshes)),
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
	m_ModelMatrix = std::move(other.m_ModelMatrix);
	m_NormalMatrix = std::move(other.m_NormalMatrix);
	m_Shader = other.m_Shader;
}

void Model::draw() const {
	m_Shader->setUniformMat3f("u_NormalMatrix", m_NormalMatrix);
	m_Shader->setUniformMat4f("u_Model", m_ModelMatrix);

	for (const auto& mesh : m_Meshes) {
		mesh.draw();
	}
}

void Model::setModelMatrix(const glm::mat4& model) {
	m_ModelMatrix = model;
	m_NormalMatrix = glm::transpose(glm::inverse(m_ModelMatrix));
}
#include "Model.hpp"
#include <iostream>

Model::Model(BatchManager& batcher, Shader* shader) :
	m_Batcher(std::move(batcher)),
	m_Shader(shader),
	m_ModelMatrix(glm::mat4(1.0f)),
	m_NormalMatrix(glm::mat3(1.0f)) { }

Model::Model(Model&& other) noexcept {
	*this = std::move(other);
}

Model& Model::operator=(Model&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	m_Batcher = std::move(other.m_Batcher);

	m_ModelMatrix = std::move(other.m_ModelMatrix);
	m_NormalMatrix = std::move(other.m_NormalMatrix);
	m_Shader = other.m_Shader;

	return *this;
}

void Model::draw() const {
	m_Shader->bind();
	m_Shader->setUniformMat3f("u_NormalMatrix", m_NormalMatrix);
	m_Shader->setUniformMat4f("u_Model", m_ModelMatrix);
	m_Batcher.draw();
}

void Model::setModelMatrix(const glm::mat4& model) {
	m_ModelMatrix = model;
	m_NormalMatrix = glm::transpose(glm::inverse(m_ModelMatrix));
}
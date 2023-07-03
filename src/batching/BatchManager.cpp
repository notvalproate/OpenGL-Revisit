#include "BatchManager.hpp"
#include <iostream>

static const std::size_t maxMaterialsPerBatch = 8;

BatchManager::BatchManager(BatchManager&& other) noexcept {
	*this = std::move(other);
}

BatchManager& BatchManager::operator=(BatchManager&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	m_Shader = other.m_Shader;
	m_Batches = std::move(other.m_Batches);
	m_Materials = std::move(other.m_Materials);
}

void BatchManager::add(std::size_t materialIndex, Mesh& mesh) {
	std::size_t batchIndex = materialIndex / maxMaterialsPerBatch;

	if (m_Batches.size() < batchIndex + 1) {
		m_Batches.push_back(Batch());
	}

	m_Batches[batchIndex].add(mesh);
}

void BatchManager::finalize(std::vector<Material>& materials, Shader* shader) {
	m_Shader = shader;
	m_Materials = std::move(materials);

	for (auto& batch : m_Batches) {
		batch.finalize(shader);
	}
}

void BatchManager::clean() {
	m_Batches.clear();
	m_Materials.clear();
}

void BatchManager::draw() const {
	size_t materialBatch = 0;
	size_t textureCount = 0;

	for (const auto& batch : m_Batches) {
		for (size_t i = 0; i < maxMaterialsPerBatch && i < m_Materials.size(); i++) {
			m_Materials[i + (materialBatch * maxMaterialsPerBatch)].bind(m_Shader, textureCount);
		}
		batch.draw();

		materialBatch++;
	}
}
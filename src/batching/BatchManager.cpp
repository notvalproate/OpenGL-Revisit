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

	return *this;
}

void BatchManager::add(std::size_t materialIndex, Mesh& mesh) {
	std::size_t batchIndex = materialIndex / maxMaterialsPerBatch;
	int batchSizeDifference = batchIndex - m_Batches.size() + 1;

	for (int i = 0; i < batchSizeDifference; i++) {
		m_Batches.push_back(Batch());
	}

	m_Batches[batchIndex].add(mesh);
}

void BatchManager::finalize(std::vector<Material>& materials, Shader* shader) {
	if (m_Finalized) {
		std::cout << "Already Finalized this batch manager!" << std::endl;
		return;
	}

	m_Shader = shader;
	m_Materials = std::move(materials);

	for (auto& batch : m_Batches) {
		batch.finalize(shader);
	}
	m_Finalized = true;
}

void BatchManager::clean() {
	m_Batches.clear();
	m_Materials.clear();
	m_Finalized = false;
}

void BatchManager::draw() const {
	std::size_t materialBatch = 0;

	for (const auto& batch : m_Batches) {
		bindMaterialBatch(materialBatch++);
		batch.draw();
	}
}

void BatchManager::bindMaterialBatch(std::size_t materialBatch) const {
	std::size_t textureCount = 0;
	std::size_t i = 0;
	std::size_t materialStartingIndex = materialBatch * maxMaterialsPerBatch;
	while (materialStartingIndex + i < m_Materials.size() && i < 8) {
		m_Materials[materialStartingIndex + i].bind(m_Shader, textureCount);
		i++;
	}
}
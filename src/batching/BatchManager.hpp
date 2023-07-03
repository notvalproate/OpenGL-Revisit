#pragma once
#include "Batch.hpp"

#include "../rendering/Shader.hpp"

#include <vector>

class BatchManager {
public:
	BatchManager() = default;

	void add(std::size_t index, std::vector<float>& vertices, std::vector<unsigned int>& indices);
	void finalize(std::vector<std::unique_ptr<Material>>& m_Materials);

	void draw(Shader* shader);
private:
	std::vector<Batch> m_Batches{};
};
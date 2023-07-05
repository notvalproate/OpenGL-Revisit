#pragma once
#include "Batch.hpp"

#include "../rendering/Shader.hpp"
#include "../modeling/Mesh.hpp"

#include <vector>

class BatchManager {
public:
	BatchManager() = default;

	BatchManager(const BatchManager& other) = delete;
	BatchManager& operator=(const BatchManager& other) = delete;

	BatchManager(BatchManager&& other) noexcept;
	BatchManager& operator=(BatchManager&& other) noexcept;

	void add(std::size_t materialIndex, Mesh& mesh);
	void finalize(std::vector<Material>& materials, Shader* shader);
	void clean();

	void draw() const;
private:
	std::vector<Batch> m_Batches{};
	std::vector<Material> m_Materials{};
	Shader* m_Shader;

	bool m_Finalized{ false };

	void bindMaterialBatch(std::size_t materialBatch) const;
};
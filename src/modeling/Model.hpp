#pragma once
#include <filesystem>
#include <vector>

#include "../rendering/Shader.hpp"
#include "Mesh.hpp"
#include "../batching/BatchManager.hpp"

class Model {
public:
	Model(std::vector<std::unique_ptr<Mesh>>& meshes, std::vector<Material>& materials, Shader* shader);
	~Model() = default;

	Model(const Model& other) = delete;
	Model& operator=(const Model& other) = delete;

	Model(Model&& other) noexcept;
	Model& operator=(Model&& other) noexcept;

	void draw() const;
	void setModelMatrix(const glm::mat4& model);
private:
	BatchManager m_Batcher;

	std::vector<std::unique_ptr<Mesh>> m_Meshes;
	std::vector<Material> m_Materials;
	glm::mat4 m_ModelMatrix;
	glm::mat3 m_NormalMatrix;
	Shader* m_Shader;
};
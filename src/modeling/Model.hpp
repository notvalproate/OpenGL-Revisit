#pragma once
#include <filesystem>
#include <vector>

#include "../rendering/Shader.hpp"
#include "Mesh.hpp"
#include "../batching/BatchManager.hpp"

class Model {
public:
	Model(BatchManager& batcher, Shader* shader);
	~Model() = default;

	Model(const Model& other) = delete;
	Model& operator=(const Model& other) = delete;

	Model(Model&& other) noexcept;
	Model& operator=(Model&& other) noexcept;

	void draw() const;
	void setModelMatrix(const glm::mat4& model);
private:
	BatchManager m_Batcher;

	glm::mat4 m_ModelMatrix;
	glm::mat3 m_NormalMatrix;

	Shader* m_Shader;
};
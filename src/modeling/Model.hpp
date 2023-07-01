#pragma once
#include <filesystem>
#include <vector>

#include "../rendering/Shader.hpp"
#include "Mesh.hpp"

class Model {
public:
	Model(std::vector<Mesh>& meshes, Shader* shader);
	~Model() = default;

	Model(const Model& other) = delete;
	Model& operator=(const Model& other) = delete;

	Model(Model&& other) noexcept;
	Model& operator=(Model&& other) noexcept;

	void draw() const;
	void setModelMatrix(const glm::mat4& model);
private:
	std::vector<Mesh> m_Meshes;
	glm::mat4 m_ModelMatrix;
	glm::mat3 m_NormalMatrix;
	Shader* m_Shader;
};
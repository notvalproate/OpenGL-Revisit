#pragma once
#include <filesystem>
#include <vector>

#include "../rendering/Shader.hpp"
#include "Mesh.hpp"
#include "../textures/Texture2D.hpp"
#include "../textures/Material.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
	Model(const std::filesystem::path modelPath, Shader* shader, bool flipUVs);
	~Model() = default;

	Model(const Model& other) = delete;
	Model& operator=(const Model& other) = delete;

	Model(Model&& other) noexcept;
	Model& operator=(Model&& other) noexcept;

	void draw() const;
	void setModelMatrix(const glm::mat4& model);
private:
	std::vector<Mesh> m_Meshes;
	std::vector<std::shared_ptr<Texture2D>> m_LoadedTextures;
	std::vector<std::shared_ptr<Material>> m_LoadedMaterials;

	glm::mat4 m_ModelMatrix;
	glm::mat3 m_NormalMatrix;

	std::filesystem::path m_Directory;
	Shader* m_Shader;

	void loadModel(const std::filesystem::path modelPath, bool flipUVs);

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void processVertex(int index, aiMesh* mesh, std::vector<float>& vertices, const VertexLayout& layout) const;

	std::shared_ptr<Material> processMaterial(aiMesh* mesh, const aiScene* scene);
	std::shared_ptr<Texture2D> loadMaterialTexture(aiMaterial* meshmaterial, aiTextureType type, TextureType typeName);
};
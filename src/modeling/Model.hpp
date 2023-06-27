#pragma once
#include <filesystem>
#include <vector>

#include "../rendering/Shader.hpp"
#include "Mesh.hpp"
#include "../textures/Texture2D.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
	Model(const std::filesystem::path modelPath, Shader* shader);

	void draw() const;
	void setModelMatrix(const glm::mat4& model);
private:
	std::vector<Mesh> m_Meshes;
	std::vector<Texture2D> m_LoadedTextures;
	std::filesystem::path m_Directory;
	glm::mat4 m_ModelMatrix;
	Shader* m_Shader;

	void loadModel(const std::filesystem::path modelPath);

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void processVertex(int index, aiMesh* mesh, std::vector<float>& vertices, VertexLayout& layout) const;
	void processMaterial(aiMesh* mesh, const aiScene* scene, std::vector<Texture2D>& textures);

	std::vector<Texture2D> loadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName);
};
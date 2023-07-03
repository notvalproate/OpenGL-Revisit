#pragma once
#include "../textures/Texture2D.hpp"
#include "../textures/Material.hpp"
#include "../rendering/VertexLayout.hpp"

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.hpp"
#include "../batching/BatchManager.hpp"

class ModelLoader {
public:
	ModelLoader() = default;

	Model loadModel(const std::filesystem::path modelPath, Shader* shader, bool flipUVs);
private:
	BatchManager m_Batcher;

	std::vector<Material> m_LoadedMaterials{};
	std::vector<std::shared_ptr<Texture2D>> m_LoadedTextures{};

	std::filesystem::path m_Directory;
	Shader* m_Shader;

	void cleanLoader();
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void processVertex(std::size_t index, aiMesh* mesh, std::vector<float>& vertices) const;

	void processMaterial(aiMesh* mesh, const aiScene* scene);
	void loadNewMaterial(aiMaterial* material, std::size_t materialindex);
	std::shared_ptr<Texture2D> loadMaterialTexture(aiMaterial* meshmaterial, aiTextureType type, TextureType typeName);
};
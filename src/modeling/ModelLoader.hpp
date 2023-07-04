#pragma once
#include "../textures/MaterialLoader.hpp"
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
	MaterialLoader m_MaterialLoader;

	Shader* m_Shader;

	void cleanLoader();
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void processVertex(std::size_t index, aiMesh* mesh, std::vector<float>& vertices) const;
};
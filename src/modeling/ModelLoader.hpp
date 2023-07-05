#pragma once
#include "../textures/MaterialLoader.hpp"
#include "../rendering/VertexLayout.hpp"
#include "../modeling/MeshLoader.hpp"

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
	MeshLoader m_MeshLoader;

	void cleanLoader();
	void processNode(const aiNode* node, const aiScene* scene);
};
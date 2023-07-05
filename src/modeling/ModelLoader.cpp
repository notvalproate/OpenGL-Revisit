#include "ModelLoader.hpp"

void ModelLoader::cleanLoader() {
	m_Batcher.clean();
	m_MaterialLoader.clean();
	m_MeshLoader.clean();
}

Model ModelLoader::loadModel(const std::filesystem::path modelPath, Shader* shader, bool flipUVs) {
	cleanLoader();

	Assimp::Importer importer;

	unsigned int flags = aiProcess_Triangulate;
	if (flipUVs) {
		flags = flags | aiProcess_FlipUVs;
	}

	const aiScene* scene = importer.ReadFile(modelPath.string(), flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return std::move(Model(m_Batcher, shader));
	}

	std::filesystem::path directory = modelPath.string().substr(0, modelPath.string().find_last_of('/'));

	m_MeshLoader.setLayout(shader->getLayout());

	processNode(scene->mRootNode, scene);

	m_MaterialLoader.loadMaterials(scene, directory);
	std::vector<Material> materials = m_MaterialLoader.extractMaterials();

	m_Batcher.finalize(materials, shader);

	return std::move(Model(m_Batcher, shader));
}

void ModelLoader::processNode(const aiNode* node, const aiScene* scene) {
	for (std::size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		Mesh myMesh = m_MeshLoader.loadMesh(mesh);

		m_Batcher.add(mesh->mMaterialIndex, myMesh);
	}

	for (std::size_t i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}
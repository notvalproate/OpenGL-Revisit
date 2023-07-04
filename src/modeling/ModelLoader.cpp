#include "ModelLoader.hpp"

void ModelLoader::cleanLoader() {
	m_Batcher.clean();
	m_MaterialLoader.clean();
}

Model ModelLoader::loadModel(const std::filesystem::path modelPath, Shader* shader, bool flipUVs) {
	cleanLoader();

	Assimp::Importer importer;

	unsigned int flags = aiProcess_Triangulate;
	if (flipUVs) flags = flags | aiProcess_FlipUVs;

	const aiScene* scene = importer.ReadFile(modelPath.string(), flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return std::move(Model(m_Batcher, shader));
	}

	std::filesystem::path directory = modelPath.string().substr(0, modelPath.string().find_last_of('/'));
	m_Shader = shader;

	processNode(scene->mRootNode, scene);

	m_MaterialLoader.loadMaterials(scene, directory);
	std::vector<Material> materials = m_MaterialLoader.extractMaterials();

	m_Batcher.finalize(materials, m_Shader);

	return std::move(Model(m_Batcher, m_Shader));
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene) {
	for (std::size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		Mesh myMesh = std::move(processMesh(mesh, scene));
		m_Batcher.add(mesh->mMaterialIndex, myMesh);
	}

	for (std::size_t i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
		processVertex(i, mesh, vertices);
	}

	for (std::size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (std::size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(vertices, indices);
}

void ModelLoader::processVertex(std::size_t index, aiMesh* mesh, std::vector<float>& vertices) const {
	for (const auto& attribute : m_Shader->getLayout()) {
		if (attribute == VertexAttribute::Position) {
			vertices.insert(vertices.end(), { mesh->mVertices[index].x,  mesh->mVertices[index].y, mesh->mVertices[index].z });
		}
		else if (attribute == VertexAttribute::Normal) {
			if (mesh->HasNormals()) {
				vertices.insert(vertices.end(), { mesh->mNormals[index].x,  mesh->mNormals[index].y, mesh->mNormals[index].z });
			}
			else {
				vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f });
			}
		}
		else if (attribute == VertexAttribute::TextureCoordinates) {
			if (mesh->mTextureCoords[0]) {
				vertices.insert(vertices.end(), { mesh->mTextureCoords[0][index].x,  mesh->mTextureCoords[0][index].y });
			}
			else {
				vertices.insert(vertices.end(), { 0.0f, 0.0f });
			}
		}
		else if (attribute == VertexAttribute::MaterialIndex) {
			vertices.push_back(((mesh->mMaterialIndex - 1) % 8) + 0.1f);
		}
	}
}
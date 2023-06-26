#include "Model.hpp"
#include <iostream>

Model::Model(const std::filesystem::path modelPath, Shader* shader) : m_Shader(shader) {
	loadModel(modelPath);
}

void Model::draw() {
	for (const auto mesh : m_Meshes) {
		mesh->draw();
	}
}

void Model::loadModel(const std::filesystem::path modelPath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return;
	}

	m_Directory = modelPath.string().substr(0, modelPath.string().find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (std::size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(processMesh(mesh, scene));
	}

	for (std::size_t i = 0; i < node->mNumChildren; i++) { 
		processNode(node->mChildren[i], scene);  
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture2D> textures{};

	//PROCESSING VERTICES
	for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		vertices.push_back(mesh->mNormals[i].x);
		vertices.push_back(mesh->mNormals[i].y);
		vertices.push_back(mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0]) {
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}

		vertices.push_back(0.0f);
	}

	//PROCESSING INDICES
	for (std::size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (std::size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//PROCESSING MATERIAL
	
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//std::vector<Texture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
		//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		//std::vector<Texture2D> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
		//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return new Mesh(vertices, indices, textures, m_Shader);
}

std::vector<Texture2D> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName) {
	std::vector<Texture2D> textures{};

	for (std::size_t i = 0; i < material->GetTextureCount(type); i++) {
		aiString path;
		material->GetTexture(type, i, &path);
		//textures.push_back(Texture2D(m_Directory.string() + "/" + path.C_Str(), typeName));
	}

	return textures;
}
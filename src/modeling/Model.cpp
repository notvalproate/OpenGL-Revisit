#include "Model.hpp"
#include <iostream>

Model::Model(const std::filesystem::path modelPath, Shader* shader) 
	: m_Shader(shader), m_ModelMatrix(glm::mat4(1.0f)) {
	loadModel(modelPath);
}

void Model::draw() const {
	m_Shader->setUniformMat4f("u_Model", m_ModelMatrix);

	for (const auto& mesh : m_Meshes) {
		mesh.draw();
	}
}

void Model::setModelMatrix(const glm::mat4& model) {
	m_ModelMatrix = model;
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

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture2D> textures;

	VertexLayout layout = m_Shader->getLayout();

	for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
		processVertex(i, mesh, vertices, layout);
	}

	for (std::size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (std::size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	
	if (mesh->mMaterialIndex >= 0) {
		processMaterial(mesh, scene, textures);
	}

	return Mesh(vertices, indices, textures, m_Shader);
}

void Model::processVertex(int index, aiMesh* mesh, std::vector<float>& vertices, VertexLayout& layout) const {
	for (const auto& attribute : layout.getLayoutArray()) {
		if (attribute == VertexAttribute::Position) {
			vertices.insert(vertices.end(), { mesh->mVertices[index].x,  mesh->mVertices[index].y, mesh->mVertices[index].z });
		}
		else if (attribute == VertexAttribute::Normal) {
			vertices.insert(vertices.end(), { mesh->mNormals[index].x,  mesh->mNormals[index].y, mesh->mNormals[index].z });
		}
		else if (attribute == VertexAttribute::TextureCoordinates) {
			if (mesh->mTextureCoords[0]) { 
				vertices.insert(vertices.end(), { mesh->mTextureCoords[0][index].x,  mesh->mTextureCoords[0][index].y }); 
			}
			else {
				vertices.insert(vertices.end(), { 0.0f, 0.0f }); 
			}
		}
		else {
			vertices.push_back(0.0f);
		}
	}
}

void Model::processMaterial(aiMesh* mesh, const aiScene* scene, std::vector<Texture2D>& textures) {
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture2D> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
}

std::vector<Texture2D> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName) {
	std::vector<Texture2D> textures;

	for (std::size_t i = 0; i < material->GetTextureCount(type); i++) {
		aiString path;
		material->GetTexture(type, i, &path);

		std::string pathString(m_Directory.string() + "/" + path.C_Str());

		bool skip = false;

		for (std::size_t j = 0; j < m_LoadedTextures.size(); j++) {
			if (m_LoadedTextures[j].getPath().compare(pathString) == 0) {
				textures.push_back(m_LoadedTextures[i]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			Texture2D texture(Texture2D(pathString, typeName));
			textures.push_back(texture);
			m_LoadedTextures.push_back(texture);
		}
	}

	return textures;
}
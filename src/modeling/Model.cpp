#include "Model.hpp"
#include <iostream>

Model::Model(const std::filesystem::path modelPath, Shader* shader, bool flipUVs) 
	: m_Shader(shader), m_ModelMatrix(glm::mat4(1.0f)), m_NormalMatrix(glm::transpose(glm::inverse(m_ModelMatrix))) {
	loadModel(modelPath, flipUVs);
}

Model::Model(Model&& other) noexcept {
	*this = std::move(other);
}

Model& Model::operator=(Model&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	m_Meshes = std::move(other.m_Meshes);
	m_LoadedMaterials = std::move(other.m_LoadedMaterials);
	m_LoadedTextures = std::move(other.m_LoadedTextures);

	m_ModelMatrix = std::move(other.m_ModelMatrix);
	m_NormalMatrix = std::move(other.m_NormalMatrix);

	m_Directory = std::move(other.m_Directory);
	m_Shader = other.m_Shader;
}

void Model::draw() const {
	m_Shader->setUniformMat3f("u_NormalMatrix", m_NormalMatrix);
	m_Shader->setUniformMat4f("u_Model", m_ModelMatrix);

	for (const auto& mesh : m_Meshes) {
		mesh.draw();
	}
}

void Model::setModelMatrix(const glm::mat4& model) {
	m_ModelMatrix = model;
	m_NormalMatrix = glm::transpose(glm::inverse(m_ModelMatrix));
}

void Model::loadModel(const std::filesystem::path modelPath, bool flipUVs) {
	Assimp::Importer importer;

	unsigned int flags = aiProcess_Triangulate;
	if (flipUVs) flags = flags | aiProcess_FlipUVs;

	const aiScene* scene = importer.ReadFile(modelPath.string(), flags);

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

		try {
			m_Meshes.push_back(processMesh(mesh, scene));
		}
		catch (const std::bad_alloc& e) {
			std::cerr << "Allocation for Mesh failed! Exception thrown: " << e.what() << std::endl;
		}
	}

	for (std::size_t i = 0; i < node->mNumChildren; i++) { 
		processNode(node->mChildren[i], scene);  
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::shared_ptr<Material> material;

	const VertexLayout& layout = m_Shader->getLayout();

	for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
		processVertex(i, mesh, vertices, layout);
	}

	for (std::size_t i = 0; i < mesh->mNumFaces; i++) { 
		aiFace face = mesh->mFaces[i];

		for (std::size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	
	material = processMaterial(mesh, scene);

	return Mesh(vertices, indices, material, m_Shader);
}

void Model::processVertex(std::size_t index, aiMesh* mesh, std::vector<float>& vertices, const VertexLayout& layout) const {
	for (const auto& attribute : layout) {
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
	}
}

std::shared_ptr<Material> Model::processMaterial(aiMesh* mesh, const aiScene* scene) {
	if (mesh->mMaterialIndex < 0) {
		return std::make_shared<Material>();
	}

	aiMaterial* meshmaterial = scene->mMaterials[mesh->mMaterialIndex];

	for (const std::shared_ptr<Material>& loadedmaterial : m_LoadedMaterials) {
		if (loadedmaterial->getName() == meshmaterial->GetName().C_Str()) {
			return loadedmaterial;
		}
	}

	return loadNewMaterial(meshmaterial);
}

std::shared_ptr<Material> Model::loadNewMaterial(aiMaterial* meshmaterial) {
	aiColor3D ambientColor, diffuseColor, specularColor;
	float dissolve, shininess;
	meshmaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
	meshmaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	meshmaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
	meshmaterial->Get(AI_MATKEY_OPACITY, dissolve);
	meshmaterial->Get(AI_MATKEY_SHININESS, shininess);

	std::shared_ptr<Texture2D> diffuseMap = loadMaterialTexture(meshmaterial, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
	std::shared_ptr<Texture2D> specularMap = loadMaterialTexture(meshmaterial, aiTextureType_SPECULAR, TextureType::SPECULAR);
	std::shared_ptr<Texture2D> normalMap = loadMaterialTexture(meshmaterial, aiTextureType_NORMALS, TextureType::NORMAL);

	std::shared_ptr<Material> material = std::make_shared<Material>(meshmaterial->GetName().C_Str(), ambientColor, diffuseColor, specularColor, dissolve);
	material->setDiffuseMap(diffuseMap);
	material->setSpecularMap(specularMap, shininess);
	material->setNormalMap(normalMap);

	m_LoadedMaterials.push_back(material);
	return material;
}

std::shared_ptr<Texture2D> Model::loadMaterialTexture(aiMaterial* meshmaterial, aiTextureType type, TextureType typeName) {
	if (meshmaterial->GetTextureCount(type) == 0) {
		return nullptr;
	}

	aiString path;
	meshmaterial->GetTexture(type, 0, &path);
	std::string pathString(m_Directory.string() + "/" + path.C_Str());

	for (const std::shared_ptr<Texture2D>& loadedTexture : m_LoadedTextures) {
		if (loadedTexture->getPath().compare(pathString) == 0) { 
			return loadedTexture;
		}
	}

	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(pathString);
	m_LoadedTextures.push_back(texture);

	return texture;
}
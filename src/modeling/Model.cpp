#include "Model.hpp"
#include <iostream>

Model::Model(const std::filesystem::path modelPath, Shader* shader, bool flipUVs) 
	: m_Shader(shader), m_ModelMatrix(glm::mat4(1.0f)) {
	loadModel(modelPath, flipUVs);
}

Model::~Model() {
	for (auto texture : m_LoadedTextures) {
		delete texture;
	}
	for (auto material : m_LoadedMaterials) {
		delete material;
	}
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

void Model::loadModel(const std::filesystem::path modelPath, bool flipUVs) {
	Assimp::Importer importer;

	int flags = aiProcess_Triangulate;
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
		m_Meshes.push_back(processMesh(mesh, scene));
	}

	for (std::size_t i = 0; i < node->mNumChildren; i++) { 
		processNode(node->mChildren[i], scene);  
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	Material* material = new Material();

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
		delete material; 
		material = processMaterial(mesh, scene);
	}

	return Mesh(vertices, indices, material, m_Shader);
}

void Model::processVertex(int index, aiMesh* mesh, std::vector<float>& vertices, VertexLayout& layout) const {
	for (const auto& attribute : layout.getLayoutArray()) {
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

Material* Model::processMaterial(aiMesh* mesh, const aiScene* scene) {
	aiMaterial* meshmaterial = scene->mMaterials[mesh->mMaterialIndex];

	for (const auto loadedmaterial : m_LoadedMaterials) {
		if (loadedmaterial->getName() == meshmaterial->GetName().C_Str()) {
			return loadedmaterial;
		}
	}
	
	aiColor3D ambientColor, diffuseColor, specularColor;
	float dissolve;
	meshmaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
	meshmaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	meshmaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
	meshmaterial->Get(AI_MATKEY_OPACITY, dissolve);

	Texture2D* diffuseMap = loadMaterialTexture(meshmaterial, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
	Texture2D* specularMap = loadMaterialTexture(meshmaterial, aiTextureType_SPECULAR, TextureType::SPECULAR);
	Texture2D* normalMap = loadMaterialTexture(meshmaterial, aiTextureType_NORMALS, TextureType::NORMAL);

	Material* material = new Material(meshmaterial->GetName().C_Str(), ambientColor, diffuseColor, specularColor, dissolve);
	material->setDiffuseMap(diffuseMap);
	material->setSpecularMap(specularMap);
	material->setNormalMap(normalMap);

	m_LoadedMaterials.push_back(material);

	return material;
}

Texture2D* Model::loadMaterialTexture(aiMaterial* meshmaterial, aiTextureType type, TextureType typeName) {
	if (meshmaterial->GetTextureCount(type) == 0) {
		return nullptr;
	}

	aiString path;
	meshmaterial->GetTexture(type, 0, &path);
	std::string pathString(m_Directory.string() + "/" + path.C_Str());

	for (std::size_t j = 0; j < m_LoadedTextures.size(); j++) { 
		if (m_LoadedTextures[j]->getPath().compare(pathString) == 0) { 
			return m_LoadedTextures[j];
		}
	}

	Texture2D* texture = new Texture2D(pathString, typeName);
	m_LoadedTextures.push_back(texture);

	return texture;
}
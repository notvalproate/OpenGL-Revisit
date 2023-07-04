#include "ModelLoader.hpp"
#include "Mesh.hpp"

void ModelLoader::cleanLoader() {
	m_Batcher.clean();
	m_LoadedTextures.clear();
	m_LoadedMaterials.clear();
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

	m_Directory = modelPath.string().substr(0, modelPath.string().find_last_of('/'));
	m_Shader = shader;

	processNode(scene->mRootNode, scene);
	loadMaterials(scene);

	m_Batcher.finalize(m_LoadedMaterials, m_Shader);

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

void ModelLoader::loadMaterials(const aiScene* scene) {
	for (std::size_t i = 0; i < scene->mNumMaterials; i++) {
		processMaterial(scene->mMaterials[i], i);
	}
}

void ModelLoader::processMaterial(aiMaterial* material, std::size_t materialIndex) {
	for (const auto& loadedmaterial : m_LoadedMaterials) {
		if (loadedmaterial.getName() == material->GetName().C_Str()) {
			return;
		}
	}

	loadNewMaterial(material, materialIndex);
}

void ModelLoader::loadNewMaterial(aiMaterial* meshmaterial, std::size_t materialIndex) {
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

	Material material(meshmaterial->GetName().C_Str(), (materialIndex - 1) % 8, ambientColor, diffuseColor, specularColor, dissolve);
	material.setDiffuseMap(diffuseMap);
	material.setSpecularMap(specularMap, shininess);
	material.setNormalMap(normalMap);

	m_LoadedMaterials.push_back(std::move(material));
}

std::shared_ptr<Texture2D> ModelLoader::loadMaterialTexture(aiMaterial* meshmaterial, aiTextureType type, TextureType typeName) {
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
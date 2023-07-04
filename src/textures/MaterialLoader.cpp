#include "MaterialLoader.hpp"

void MaterialLoader::loadMaterials(const aiScene* scene, const std::filesystem::path& directory) {
	m_Directory = directory;

	for (std::size_t i = 0; i < scene->mNumMaterials; i++) {
		processMaterial(scene->mMaterials[i], i);
	}
}

void MaterialLoader::clean() {
	m_LoadedMaterials.clear();
	m_LoadedTextures.clear();
	m_Directory.clear();
}

void MaterialLoader::processMaterial(aiMaterial* material, std::size_t materialIndex) {
	MaterialProperties properties;
	aiColor3D ambientColor, diffuseColor, specularColor;

	material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
	material->Get(AI_MATKEY_OPACITY, properties.dissolve);
	material->Get(AI_MATKEY_SHININESS, properties.shininess);

	properties.ambient = {ambientColor.r, ambientColor.g, ambientColor.b};
	properties.diffuse = {diffuseColor.r, diffuseColor.g, diffuseColor.b};
	properties.specular = {specularColor.r, specularColor.g, specularColor.b};

	properties.diffuseMap = getTexture(material, aiTextureType_DIFFUSE);
	properties.specularMap = getTexture(material, aiTextureType_SPECULAR);
	properties.normalMap = getTexture(material, aiTextureType_NORMALS);

	Material myMaterial((materialIndex - 1) % 8, properties);

	m_LoadedMaterials.push_back(std::move(myMaterial));
}

std::shared_ptr<Texture2D> MaterialLoader::getTexture(aiMaterial* material, aiTextureType type) {
	if (material->GetTextureCount(type) == 0) {
		return nullptr;
	}

	aiString path;
	material->GetTexture(type, 0, &path);
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
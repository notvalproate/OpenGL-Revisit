#pragma once
#include "Material.hpp"
#include "Texture2D.hpp"

#include <assimp/scene.h>

#include <vector>

class MaterialLoader {
public:
	MaterialLoader() = default;

	void loadMaterials(const aiScene* scene, const std::filesystem::path& directory);
	void clean();

	std::vector<Material> extractMaterials() { return std::move(m_LoadedMaterials); }
private:
	std::vector<Material> m_LoadedMaterials{};
	std::vector<std::shared_ptr<Texture2D>> m_LoadedTextures{};
	std::filesystem::path m_Directory;

	void processMaterial(aiMaterial* material, std::size_t materialIndex);
	std::shared_ptr<Texture2D> getTexture(aiMaterial* meshmaterial, aiTextureType type);
};
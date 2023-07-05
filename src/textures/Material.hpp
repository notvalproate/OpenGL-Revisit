#pragma once
#include "Texture2D.hpp"
#include "../rendering/Shader.hpp" 

#include <glm/glm.hpp>

#include <assimp/scene.h>

struct MaterialProperties {
	glm::vec3 ambient{};
	glm::vec3 diffuse{};
	glm::vec3 specular{};

	float dissolve{};
	float shininess{};

	std::shared_ptr<Texture2D> diffuseMap;
	std::shared_ptr<Texture2D> specularMap;
	std::shared_ptr<Texture2D> normalMap;
};

class Material {
public:
	Material(std::size_t index, MaterialProperties& properties);

	void bind(Shader* shader, std::size_t& textureCount) const;
private:
	std::string m_MaterialIndex;
	MaterialProperties m_Properties;
};
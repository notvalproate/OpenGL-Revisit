#pragma once
#include "Texture2D.hpp"
#include "../rendering/Shader.hpp" 

#include <glm/glm.hpp>

#include <assimp/scene.h>

class Material {
public:
	Material(const std::string& name, std::size_t index, const aiColor3D& ambient, const aiColor3D& diffuse, const aiColor3D& specular, float dissolve);

	void bind(Shader* shader) const;
	void unbind(Shader* shader) const;

	void setDiffuseMap(std::shared_ptr<Texture2D> map);
	void setSpecularMap(std::shared_ptr<Texture2D> map, float shininess);
	void setNormalMap(std::shared_ptr<Texture2D> map);

	[[nodiscard]] std::string_view getName() const { return m_Name; }
private:
	std::string m_Name;
	std::string m_MaterialIndex;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Dissolve;
	float m_Shininess;

	std::shared_ptr<Texture2D> m_DiffuseMap;
	std::shared_ptr<Texture2D> m_SpecularMap;
	std::shared_ptr<Texture2D> m_NormalMap;
};
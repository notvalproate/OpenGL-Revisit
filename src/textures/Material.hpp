#pragma once
#include "Texture2D.hpp"
#include "../rendering/Shader.hpp" 

#include <glm/glm.hpp>

#include <assimp/scene.h>

class Material {
public:
	Material();
	Material(const std::string& name, const aiColor3D& ambient, const aiColor3D& diffuse, const aiColor3D& specular, float dissolve);

	void bind(Shader* shader) const;
	void unbind(Shader* shader) const;

	void setDiffuseMap(Texture2D* map);
	void setSpecularMap(Texture2D* map, float shininess);
	void setNormalMap(Texture2D* map);

	Texture2D* getdiffusemap() const { return m_DiffuseMap; }
	[[nodiscard]] std::string getName() const { return m_Name; }
private:
	std::string m_Name;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Dissolve;
	float m_Shininess;

	Texture2D* m_DiffuseMap;
	Texture2D* m_SpecularMap;
	Texture2D* m_NormalMap;
};
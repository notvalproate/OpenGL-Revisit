#include "Material.hpp"
#include <iostream>

Material::Material(const std::string& name, std::size_t index, const aiColor3D& ambient, const aiColor3D& diffuse, const aiColor3D& specular, float dissolve) :
	m_Name(name),
	m_MaterialIndex("u_Material[" + std::to_string(index) + "]."),
	m_Ambient(ambient.r, ambient.g, ambient.b),
	m_Diffuse(diffuse.r, diffuse.g, diffuse.b),
	m_Specular(specular.r, specular.g, specular.b),
	m_Dissolve(dissolve),
	m_Shininess(32.0f) { }

void Material::bind(Shader* shader) const {
	shader->setUniform3fv("u_Material.ambient", m_Ambient);
	shader->setUniform3fv("u_Material.diffuse", m_Diffuse);
	shader->setUniform3fv("u_Material.specular", m_Specular);
	shader->setUniform1f("u_Material.dissolve", m_Dissolve);
	shader->setUniform1f("u_Material.shininess", m_Shininess);

	if (m_DiffuseMap) {
		shader->setUniform1i("u_Material.diffuseMap", 0);
		m_DiffuseMap->bind(0);
		shader->setUniform1i("u_Material.hasDiffuse", 1);
	}
	if (m_SpecularMap) {
		shader->setUniform1i("u_Material.specularMap", 1);
		m_SpecularMap->bind(1);
		shader->setUniform1i("u_Material.hasSpecular", 1);
	}
	if (m_NormalMap) {
		shader->setUniform1i("u_Material.normalMap", 2);
		m_NormalMap->bind(2);
		shader->setUniform1i("u_Material.hasNormal", 1);
	}
}

void Material::unbind(Shader* shader) const {
	shader->setUniform1i("u_Material.hasDiffuse", 0);
	shader->setUniform1i("u_Material.hasSpecular", 0);
	shader->setUniform1i("u_Material.hasNormal", 0);
}

void Material::setDiffuseMap(std::shared_ptr<Texture2D> map) {
	m_DiffuseMap = map; 
}

void Material::setSpecularMap(std::shared_ptr<Texture2D> map, float shininess) {
	m_SpecularMap = map; 
	m_Shininess = shininess;
}

void Material::setNormalMap(std::shared_ptr<Texture2D> map) {
	m_NormalMap = map; 
}
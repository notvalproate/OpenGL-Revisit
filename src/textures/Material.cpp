#include "Material.hpp"
#include <iostream>

Material::Material(std::size_t index, MaterialProperties& properties) :
	m_MaterialIndex("u_Material[" + std::to_string(index) + "]."),
	m_Properties(std::move(properties)) { }

void Material::bind(Shader* shader, std::size_t& textureCount) const {
	shader->setUniform3fv(m_MaterialIndex + "ambient", m_Properties.ambient);
	shader->setUniform3fv(m_MaterialIndex + "diffuse", m_Properties.diffuse);
	shader->setUniform3fv(m_MaterialIndex + "specular", m_Properties.specular);
	shader->setUniform1f(m_MaterialIndex + "dissolve", m_Properties.dissolve);
	shader->setUniform1f(m_MaterialIndex + "shininess", m_Properties.shininess);

	if (m_Properties.diffuseMap) {
		shader->setUniform1i(m_MaterialIndex + "diffuseMap", textureCount);
		m_Properties.diffuseMap->bind(textureCount++);
		shader->setUniform1i(m_MaterialIndex + "hasDiffuse", 1);
	}
	else {
		shader->setUniform1i(m_MaterialIndex + "hasDiffuse", 0);
	}
	if (m_Properties.specularMap) {
		shader->setUniform1i(m_MaterialIndex + "specularMap", textureCount);
		m_Properties.specularMap->bind(textureCount++);
		shader->setUniform1i(m_MaterialIndex + "hasSpecular", 1);
	}
	else {
		shader->setUniform1i(m_MaterialIndex + "hasSpecular", 0);
	}
	if (m_Properties.normalMap) {
		shader->setUniform1i(m_MaterialIndex + "normalMap", textureCount);
		m_Properties.normalMap->bind(textureCount++);
		shader->setUniform1i(m_MaterialIndex + "hasNormal", 1);
	}
	else {
		shader->setUniform1i(m_MaterialIndex + "hasNormal", 0);
	}
}
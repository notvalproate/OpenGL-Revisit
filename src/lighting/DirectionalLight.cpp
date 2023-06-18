#include "DirectionalLight.hpp"

DirectionalLight DirectionalLight::s_Instance;

DirectionalLight::DirectionalLight() : m_Shader(nullptr) {}

DirectionalLight& DirectionalLight::getDirectionalLight() {
	return s_Instance;
}

void DirectionalLight::setShader(Shader* shader) {
	m_Shader = shader;
}

void DirectionalLight::setDirectionalLight(glm::vec3 direction, glm::vec3 color, float brightness) {
	m_Shader->setUniform3fv("u_DirectionalLight.Direction", direction);
	m_Shader->setUniform3fv("u_DirectionalLight.Ambient", 0.2f * color);
	m_Shader->setUniform3fv("u_DirectionalLight.Diffuse", color);
	m_Shader->setUniform3fv("u_DirectionalLight.Specular", 0.6f * color);
	m_Shader->setUniform1f("u_DirectionalLight.Brightness", brightness);
}
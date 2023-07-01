#include "FlashLight.hpp"

FlashLight FlashLight::s_Instance;

FlashLight::FlashLight() : m_Shader(nullptr), m_Camera(nullptr) { }

FlashLight& FlashLight::getFlashLight() {
	return s_Instance;
}

void FlashLight::setShaderAndCamera(Shader* shader, Camera* camera) {
    m_Shader = shader;
    m_Camera = camera;
}

void FlashLight::setFlashLight(glm::vec3 color, float innerCutOff, float outerCutOff, float brightess) {
    m_Shader->setUniform3fv("u_SpotLight.Ambient", 0.1f * color);
    m_Shader->setUniform3fv("u_SpotLight.Diffuse", color);
    m_Shader->setUniform3fv("u_SpotLight.Specular", 0.5f * color);
    m_Shader->setUniform1f("u_SpotLight.InnerCutOff", glm::cos(glm::radians(innerCutOff)));
    m_Shader->setUniform1f("u_SpotLight.OuterCutOff", glm::cos(glm::radians(outerCutOff)));
    m_Shader->setUniform1f("u_SpotLight.Kc", 1.0f);
    m_Shader->setUniform1f("u_SpotLight.Kl", 0.07f);
    m_Shader->setUniform1f("u_SpotLight.Kq", 0.014f);
    m_Shader->setUniform1f("u_SpotLight.Brightness", brightess);
}

void FlashLight::update() {
    m_Shader->setUniform3fv("u_SpotLight.Position", m_Camera->getPosition());
    m_Shader->setUniform3fv("u_SpotLight.Direction", m_Camera->getDirection());
}
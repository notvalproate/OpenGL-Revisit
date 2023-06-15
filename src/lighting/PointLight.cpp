#include "PointLight.hpp"
#include <iostream>

PointLight::PointLight(const glm::vec3& p_Position, const glm::vec3& p_Color, float p_Brightness)
	: m_Position(p_Position)
    , m_Ambient(0.2f * p_Color)
    , m_Diffusion(p_Color)
    , m_Specular(p_Color)
    , m_Brightness(p_Brightness) { }

void PointLight::UpdateUniforms(Shader& p_Shader) {
    p_Shader.SetUniform3fv("u_PointLight.Position", m_Position);
    p_Shader.SetUniform3fv("u_PointLight.Ambient", m_Ambient);
    p_Shader.SetUniform3fv("u_PointLight.Diffuse", m_Diffusion);
    p_Shader.SetUniform3fv("u_PointLight.Specular", m_Specular);
    p_Shader.SetUniform1f("u_PointLight.Kc", Kc);
    p_Shader.SetUniform1f("u_PointLight.Kl", Kl);
    p_Shader.SetUniform1f("u_PointLight.Kq", Kq);
    p_Shader.SetUniform1f("u_PointLight.Brightness", m_Brightness);
}

void PointLight::SetPosition(const glm::vec3& p_Position, Shader& p_Shader) {
    m_Position = p_Position;
    p_Shader.SetUniform3fv("u_PointLight.Position", m_Position);
}

void PointLight::Translate(const glm::vec3& p_Translation, Shader& p_Shader) {
    m_Position += p_Translation;
    p_Shader.SetUniform3fv("u_PointLight.Position", m_Position);
} 

void PointLight::SetColor(const glm::vec3& p_Color) {
    m_Ambient = 0.2f * p_Color;
    m_Diffusion = p_Color;
    m_Specular = p_Color;
}

void PointLight::SetAttenuation(float p_Constant, float p_Linear, float p_Quadratic) {
    Kc = p_Constant; 
    Kl = p_Linear;
    Kq = p_Quadratic;
}
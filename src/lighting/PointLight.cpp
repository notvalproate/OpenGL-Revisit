#include "PointLight.hpp"
#include <iostream> 
#include <glm/gtc/matrix_transform.hpp>

PointLight::PointLight(unsigned short a_Index, const glm::vec3& a_Position, const glm::vec3& a_Color, float a_Brightness, Shader* a_Shader, Shader* a_ModelShader) 
    : m_Index("u_PointLight[" + std::to_string(a_Index) + "]."), m_Model(glm::mat4(1.0f))
{
    m_Model = glm::scale(glm::translate(m_Model, a_Position), glm::vec3(0.2f)); 
    m_ModelShader = a_ModelShader;
    m_ModelShader->setUniformMat4f("u_Model", m_Model);

    m_ModelColor = a_Color;
    m_ModelShader->setUniform3fv("u_Color", m_ModelColor);

    a_Shader->setUniform3fv(m_Index + "Position", a_Position);
    a_Shader->setUniform3fv(m_Index + "Ambient", 0.2f * a_Color);
    a_Shader->setUniform3fv(m_Index + "Diffuse", a_Color);
    a_Shader->setUniform3fv(m_Index + "Specular", 0.5f * a_Color);
    a_Shader->setUniform1f(m_Index + "Kc", 1.0f);
    a_Shader->setUniform1f(m_Index + "Kl", 0.07f);
    a_Shader->setUniform1f(m_Index + "Kq", 0.017f);
    a_Shader->setUniform1f(m_Index + "Brightness", a_Brightness);
}

void PointLight::resetUniforms(Shader* a_Shader) {
    a_Shader->setUniform3fv(m_Index + "Position", glm::vec3(0.0f));
    a_Shader->setUniform3fv(m_Index + "Ambient", glm::vec3(0.0f));
    a_Shader->setUniform3fv(m_Index + "Diffuse", glm::vec3(0.0f));
    a_Shader->setUniform3fv(m_Index + "Specular", glm::vec3(0.0f));
    a_Shader->setUniform1f(m_Index + "Kc", 0.0f);
    a_Shader->setUniform1f(m_Index + "Kl", 0.0f);
    a_Shader->setUniform1f(m_Index + "Kq", 0.0f);
    a_Shader->setUniform1f(m_Index + "Brightness", 0.0f);
}

void PointLight::setPosition(const glm::vec3& a_Position, Shader* a_Shader) {
    m_Model = glm::scale(glm::translate(glm::mat4(1.0f), a_Position), glm::vec3(0.2f));
    m_ModelShader->setUniformMat4f("u_Model", m_Model);
    m_ModelShader->setUniform3fv("u_Color", m_ModelColor);

    a_Shader->setUniform3fv(m_Index + "Position", a_Position);
}

PointLightList PointLightList::s_Instance;

void PointLightList::addLight(unsigned short a_Index, const glm::vec3& a_Position, const glm::vec3& a_Color, float a_Brightness, Shader* a_ModelShader) {
    if (a_Index >= 50) {
        std::cout << "PointLight index cannot be more than 50!" << std::endl;
        __debugbreak();
        return;
    }

    if (m_PointLights.contains(a_Index)) {
        std::cout << "PointLight already exists at index " << a_Index << std::endl;
        return;
    }

    m_PointLights[a_Index] = new PointLight(a_Index, a_Position, a_Color, a_Brightness, m_Shader, a_ModelShader);
}

void PointLightList::removeLight(unsigned short a_Index) {
    if (m_PointLights.contains(a_Index)) {
        auto pointLight = m_PointLights.at(a_Index);
        pointLight->resetUniforms(m_Shader);
        delete pointLight;
        m_PointLights.erase(a_Index);
        return;
    }

    std::cout << "No PointLight at index  " << a_Index << std::endl;
}

void PointLightList::setShader(Shader* a_Shader) {
    m_Shader = a_Shader;
}

void PointLightList::setLightPosition(unsigned short a_Index, const glm::vec3& a_Position) {
    if (m_PointLights.contains(a_Index)) {
        auto pointLight = m_PointLights.at(a_Index);
        pointLight->setPosition(a_Position, m_Shader);
        return;
    }
}
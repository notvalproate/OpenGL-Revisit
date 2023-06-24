#include "PointLight.hpp"
#include <iostream> 
#include <glm/gtc/matrix_transform.hpp>

PointLight::PointLight(unsigned short index, const glm::vec3& position, const glm::vec3& color, float brightness, Shader* shader, Shader* modelShader) 
    : m_Index("u_PointLight[" + std::to_string(index) + "]."), m_Model(glm::mat4(1.0f))
{
    m_Model = glm::scale(glm::translate(m_Model, position), glm::vec3(0.2f)); 
    m_ModelShader = modelShader;
    m_ModelShader->setUniformMat4f("u_Model", m_Model);

    m_ModelColor = color;
    m_ModelShader->setUniform3fv("u_Color", m_ModelColor);

    shader->setUniform3fv(m_Index + "Position", position);
    shader->setUniform3fv(m_Index + "Ambient", 0.2f * color);
    shader->setUniform3fv(m_Index + "Diffuse", color);
    shader->setUniform3fv(m_Index + "Specular", 0.5f * color);
    shader->setUniform1f(m_Index + "Kc", 1.0f);
    shader->setUniform1f(m_Index + "Kl", 0.07f);
    shader->setUniform1f(m_Index + "Kq", 0.017f);
    shader->setUniform1f(m_Index + "Brightness", brightness);
}

void PointLight::resetUniforms(Shader* shader) {
    shader->setUniform3fv(m_Index + "Position", glm::vec3(0.0f));
    shader->setUniform3fv(m_Index + "Ambient", glm::vec3(0.0f));
    shader->setUniform3fv(m_Index + "Diffuse", glm::vec3(0.0f));
    shader->setUniform3fv(m_Index + "Specular", glm::vec3(0.0f));
    shader->setUniform1f(m_Index + "Kc", 0.0f);
    shader->setUniform1f(m_Index + "Kl", 0.0f);
    shader->setUniform1f(m_Index + "Kq", 0.0f);
    shader->setUniform1f(m_Index + "Brightness", 0.0f);
}

void PointLight::setPosition(const glm::vec3& position, Shader* shader) {
    m_Model = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(0.2f));
    m_ModelShader->setUniformMat4f("u_Model", m_Model);
    m_ModelShader->setUniform3fv("u_Color", m_ModelColor);

    shader->setUniform3fv(m_Index + "Position", position);
}

PointLightList PointLightList::s_Instance;

PointLightList& PointLightList::getList() {
    return s_Instance;
}

PointLightList::PointLightList() : m_PointLights({}), m_Shader(nullptr) {}

void PointLightList::addLight(unsigned short index, const glm::vec3& position, const glm::vec3& color, float brightness, Shader* modelShader) {
    if (index >= 50) {
        std::cout << "PointLight index cannot be more than 50!" << std::endl;
        __debugbreak();
        return;
    }

    if (m_PointLights.contains(index)) {
        std::cout << "PointLight already exists at index " << index << std::endl;
        return;
    }

    m_PointLights[index] = new PointLight(index, position, color, brightness, m_Shader, modelShader);
}

void PointLightList::removeLight(unsigned short index) {
    if (m_PointLights.contains(index)) {
        auto pointLight = m_PointLights.at(index);
        pointLight->resetUniforms(m_Shader);
        delete pointLight;
        m_PointLights.erase(index);
        return;
    }

    std::cout << "No PointLight at index  " << index << std::endl;
}

void PointLightList::setShader(Shader* Shader) {
    m_Shader = Shader;
}

void PointLightList::setLightPosition(unsigned short index, const glm::vec3& position) {
    if (m_PointLights.contains(index)) {
        auto pointLight = m_PointLights.at(index);
        pointLight->setPosition(position, m_Shader);
        return;
    }
}
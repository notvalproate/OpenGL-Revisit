#pragma once
#include <glm/glm.hpp>
#include "../rendering/Shader.hpp"

class PointLight {
public:
	PointLight(const glm::vec3& p_Position, const glm::vec3& p_Color, float p_Brightness);

	void UpdateUniforms(Shader& p_Shader);

	void SetPosition(const glm::vec3& p_Position, Shader& p_Shader);
	void Translate(const glm::vec3& p_Translation, Shader& p_Shader);

	void SetColor(const glm::vec3& p_Color);
	void SetAttenuation(float p_Constant, float p_Linear, float p_Quadratic);
private:
	glm::vec3 m_Position;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffusion; 
	glm::vec3 m_Specular;  

	float Kc = 1.0f, Kl = 0.14f, Kq = 0.07f;
	float m_Brightness;
};
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
	: m_Fov(70.0f)
	, m_Speed(2.0f)
	, m_Sensitivity(0.1f)

	, m_Position(0.0f, 0.0f, 3.0f)
	, m_Direction(0.0f, 0.0f, -1.0f)
	, m_Up(0.0f, 1.0f, 0.0f) 
{
	m_Projection = glm::perspective(glm::radians(m_Fov), (float)16 / (float)9, 0.1f, 100.0f);
	m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
}

Camera::Camera(const float p_Speed, const float p_Sensitivity, const float p_Fov) : Camera() {
	m_Fov = p_Fov;
	m_Speed = p_Speed;
	m_Sensitivity = p_Sensitivity;
	m_Projection = glm::perspective(glm::radians(m_Fov), (float)16 / (float)9, 0.1f, 100.0f);
}

void Camera::UpdateUniforms(std::string_view p_View, std::string_view p_Projection, Shader& p_Shader) {
	m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
	p_Shader.SetUniformMat4f(p_View, m_View);
	p_Shader.SetUniformMat4f(p_Projection, m_Projection);
}
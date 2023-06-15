#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
	: m_Fov(70.0f)
	, m_Speed(4.0f)
	, m_SpeedMultiplier(2.0f)
	, m_Sensitivity(0.1f)
	, m_Yaw(-90.0)
	, m_Pitch(0.0)

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

void Camera::UpdateUniforms(std::string_view p_View, std::string_view p_Projection, std::string_view p_ViewPos, Shader& p_Shader) {
	UpdateUniforms(p_View, p_Projection, p_Shader);
	p_Shader.SetUniform3fv(p_ViewPos, m_Position);
}

void Camera::ResetProjectionMat(int p_Width, int p_Height) {
	m_Projection = glm::perspective(glm::radians(m_Fov), (float)p_Width / (float)p_Height, 0.1f, 100.0f);
}
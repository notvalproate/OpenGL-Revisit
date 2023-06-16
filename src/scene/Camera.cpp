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

Camera::Camera(const float a_Speed, const float a_Sensitivity, const float a_Fov) : Camera() {
	m_Fov = a_Fov;
	m_Speed = a_Speed;
	m_Sensitivity = a_Sensitivity;
	m_Projection = glm::perspective(glm::radians(m_Fov), (float)16 / (float)9, 0.1f, 100.0f);
}

void Camera::updateUniforms(std::string_view a_View, std::string_view a_Projection, Shader& a_Shader) {
	m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
	a_Shader.setUniformMat4f(a_View, m_View);
	a_Shader.setUniformMat4f(a_Projection, m_Projection);
}

void Camera::updateUniforms(std::string_view a_View, std::string_view a_Projection, std::string_view a_ViewPos, Shader& a_Shader) {
	updateUniforms(a_View, a_Projection, a_Shader);
	a_Shader.setUniform3fv(a_ViewPos, m_Position);
}

void Camera::resetViewport(int a_Width, int a_Height) {
	m_Projection = glm::perspective(glm::radians(m_Fov), (float)a_Width / (float)a_Height, 0.1f, 100.0f);
}
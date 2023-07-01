#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : 
	m_Fov(70.0f),
	m_Speed(8.0f),
	m_SpeedMultiplier(2.0f),
	m_Sensitivity(0.1f),
	m_Yaw(-90.0),
	m_Pitch(0.0),
	
	m_Position(0.0f, 0.0f, 3.0f),
	m_Direction(0.0f, 0.0f, -1.0f),
	m_Up(0.0f, 1.0f, 0.0f) 
{
	m_Projection = glm::perspective(glm::radians(m_Fov), (float)16 / (float)9, 0.1f, 100.0f);
	m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
}

Camera::Camera(const int viewportWidth, const int viewportHeight, const float speed, const float sensitivity, const float fov) : Camera() {
	m_Fov = fov;
	m_Speed = speed;
	m_Sensitivity = sensitivity;
	m_Projection = glm::perspective(glm::radians(m_Fov), (float)viewportWidth / (float)viewportHeight, 0.1f, 100.0f);
}

Camera::Camera(const int viewportWidth, const int viewportHeight) : Camera() { 
	m_Projection = glm::perspective(glm::radians(m_Fov), (float)viewportWidth / (float)viewportHeight, 0.1f, 100.0f);
}

void Camera::updateUniforms(std::string_view view, std::string_view projection, Shader& shader) {
	m_View = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
	shader.setUniformMat4f(view, m_View);
	shader.setUniformMat4f(projection, m_Projection);
}

void Camera::updateUniforms(std::string_view view, std::string_view projection, std::string_view viewPos, Shader& shader) {
	updateUniforms(view, projection, shader);
	shader.setUniform3fv(viewPos, m_Position);
}

void Camera::resetViewport(int width, int height) {
	m_Projection = glm::perspective(glm::radians(m_Fov), (float)width / (float)height, 0.1f, 100.0f);
}
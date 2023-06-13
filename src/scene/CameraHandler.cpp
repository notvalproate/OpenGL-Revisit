#include "CameraHandler.hpp"

void CameraHandler::HandleEvents(GLFWwindow* p_Window, const float p_DeltaTime) {
	if (glfwGetKey(p_Window, GLFW_KEY_W) == GLFW_PRESS) {
		m_Camera.m_Position += m_Camera.m_Direction * m_Camera.m_Speed * p_DeltaTime;
	}
	if (glfwGetKey(p_Window, GLFW_KEY_S) == GLFW_PRESS) {
		m_Camera.m_Position -= m_Camera.m_Direction * m_Camera.m_Speed * p_DeltaTime;
	}
	if (glfwGetKey(p_Window, GLFW_KEY_A) == GLFW_PRESS) {
		m_Camera.m_Position -= glm::normalize(glm::cross(m_Camera.m_Direction, m_Camera.m_Up)) * m_Camera.m_Speed * p_DeltaTime;
	}
	if (glfwGetKey(p_Window, GLFW_KEY_D) == GLFW_PRESS) {
		m_Camera.m_Position += glm::normalize(glm::cross(m_Camera.m_Direction, m_Camera.m_Up)) * m_Camera.m_Speed * p_DeltaTime;
	}
	if (glfwGetKey(p_Window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		m_Camera.m_Position += glm::vec3(0.0f, 1.0f, 0.0f) * m_Camera.m_Speed * p_DeltaTime;
	}
	if (glfwGetKey(p_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		m_Camera.m_Position -= glm::vec3(0.0f, 1.0f, 0.0f) * m_Camera.m_Speed * p_DeltaTime;
	}
}
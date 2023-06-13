#include "CameraHandler.hpp"
#include <iostream>

void CameraHandler::HandleEvents(GLFWwindow* p_Window, const float p_DeltaTime) {
	HandleKeyboard(p_Window, p_DeltaTime);
	HandleMouse(p_Window);
}

void CameraHandler::HandleKeyboard(GLFWwindow* p_Window, const float p_DeltaTime) {
	float t_Mul = 1.0f;
	if (glfwGetKey(p_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		t_Mul = m_Camera.m_SpeedMultiplier;
	}

	if (glfwGetKey(p_Window, GLFW_KEY_W) == GLFW_PRESS) {
		m_Camera.m_Position += m_Camera.m_Direction * m_Camera.m_Speed * t_Mul * p_DeltaTime;
	}
	if (glfwGetKey(p_Window, GLFW_KEY_S) == GLFW_PRESS) {
		m_Camera.m_Position -= m_Camera.m_Direction * m_Camera.m_Speed * t_Mul * p_DeltaTime;
	}
	if (glfwGetKey(p_Window, GLFW_KEY_A) == GLFW_PRESS) {
		m_Camera.m_Position -= glm::normalize(glm::cross(m_Camera.m_Direction, m_Camera.m_Up)) * m_Camera.m_Speed * t_Mul * p_DeltaTime;
	}
	if (glfwGetKey(p_Window, GLFW_KEY_D) == GLFW_PRESS) {
		m_Camera.m_Position += glm::normalize(glm::cross(m_Camera.m_Direction, m_Camera.m_Up)) * m_Camera.m_Speed * t_Mul * p_DeltaTime;
	}
	if (glfwGetKey(p_Window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		m_Camera.m_Position += glm::vec3(0.0f, 1.0f, 0.0f) * m_Camera.m_Speed * t_Mul * p_DeltaTime;
	}
	if (glfwGetKey(p_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		m_Camera.m_Position -= glm::vec3(0.0f, 1.0f, 0.0f) * m_Camera.m_Speed * t_Mul * p_DeltaTime;
	}
}

void CameraHandler::HandleMouse(GLFWwindow* p_Window) {
	double t_Mx, t_My;
	glfwGetCursorPos(p_Window, &t_Mx, &t_My);

	if (m_FirstMouse) {
		m_MousePos.x = t_Mx;
		m_MousePos.y = t_My;
		m_FirstMouse = false;
	}

	double xoffset = (t_Mx - m_MousePos.x) * m_Camera.m_Sensitivity;
	double yoffset = (t_My - m_MousePos.y) * m_Camera.m_Sensitivity;

	m_Camera.m_Yaw += xoffset;
	m_Camera.m_Pitch -= yoffset;

	if (m_Camera.m_Pitch > 89.0f) {
		m_Camera.m_Pitch = 89.0f;
	}
	if (m_Camera.m_Pitch < -89.0f) {
		m_Camera.m_Pitch = -89.0f;
	}
	if (m_Camera.m_Yaw > 360.0f) {
		m_Camera.m_Yaw -= 360.0f;
	}
	if (m_Camera.m_Yaw < 0.0f) {
		m_Camera.m_Yaw = 360.0f - m_Camera.m_Yaw;
	}

	m_Camera.m_Direction.z = -cos(glm::radians(m_Camera.m_Yaw)) * cos(glm::radians(m_Camera.m_Pitch));
	m_Camera.m_Direction.y = sin(glm::radians(m_Camera.m_Pitch));
	m_Camera.m_Direction.x = sin(glm::radians(m_Camera.m_Yaw)) * cos(glm::radians(m_Camera.m_Pitch));

	m_Camera.m_Direction = glm::normalize(m_Camera.m_Direction);

	m_MousePos.x = t_Mx;
	m_MousePos.y = t_My;
}
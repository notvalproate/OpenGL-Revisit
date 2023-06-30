#include "CameraHandler.hpp"
#include <iostream>
#include <algorithm>

CameraHandler::CameraHandler(Camera& camera) : m_Camera(camera), m_FirstMouse(true) {
	//Initializing default keybinds
	m_KeyBindings[KeyBind::FORWARD] = GLFW_KEY_W; 
	m_KeyBindings[KeyBind::BACKWARD] = GLFW_KEY_S;
	m_KeyBindings[KeyBind::LEFT] = GLFW_KEY_A; 
	m_KeyBindings[KeyBind::RIGHT] = GLFW_KEY_D; 
	m_KeyBindings[KeyBind::UP] = GLFW_KEY_SPACE; 
	m_KeyBindings[KeyBind::DOWN] = GLFW_KEY_LEFT_SHIFT;
	m_KeyBindings[KeyBind::SPRINT] = GLFW_KEY_LEFT_CONTROL;
}

void CameraHandler::setKeybind(KeyBind control, unsigned int key) {
	m_KeyBindings[control] = key;
}

void CameraHandler::handleEvents(GLFWwindow* window, const float deltaTime) {
	handleKeyboardInput(window, deltaTime);
	handleMouseInput(window);
}

void CameraHandler::handleKeyboardInput(GLFWwindow* window, const float deltaTime) const {
	const auto pressed = [&window](unsigned int Key) {
		return glfwGetKey(window, Key) == GLFW_PRESS;
	};

	float multiplier = 1.0f;
	if (pressed(m_KeyBindings.at(KeyBind::SPRINT))) {
		multiplier = m_Camera.m_SpeedMultiplier;
	}
	
	//WASD
	if (pressed(m_KeyBindings.at(KeyBind::FORWARD))) {
		m_Camera.m_Position += m_Camera.m_Direction * m_Camera.m_Speed * multiplier * deltaTime;
	}
	if (pressed(m_KeyBindings.at(KeyBind::BACKWARD))) { 
		m_Camera.m_Position -= m_Camera.m_Direction * m_Camera.m_Speed * multiplier * deltaTime;
	}
	if (pressed(m_KeyBindings.at(KeyBind::LEFT))) {      //RIGHT VECTOR
		m_Camera.m_Position -= glm::normalize(glm::cross(m_Camera.m_Direction, m_Camera.m_Up)) * m_Camera.m_Speed * multiplier * deltaTime;
	}
	if (pressed(m_KeyBindings.at(KeyBind::RIGHT))) {
		m_Camera.m_Position += glm::normalize(glm::cross(m_Camera.m_Direction, m_Camera.m_Up)) * m_Camera.m_Speed * multiplier * deltaTime;
	}

	//SPACE AND SHIFT FOR UP AND DOWN
	if (pressed(m_KeyBindings.at(KeyBind::UP))) { 
		m_Camera.m_Position += glm::vec3(0.0f, 1.0f, 0.0f) * m_Camera.m_Speed * multiplier * deltaTime;
	}
	if (pressed(m_KeyBindings.at(KeyBind::DOWN))) { 
		m_Camera.m_Position -= glm::vec3(0.0f, 1.0f, 0.0f) * m_Camera.m_Speed * multiplier * deltaTime;
	}
}

void CameraHandler::handleMouseInput(GLFWwindow* window) {
	double x, y; 
	glfwGetCursorPos(window, &x, &y); 

	if (m_FirstMouse) {
		m_LastX = x;
		m_LastY = y; 
		m_FirstMouse = false;
	}

	//CALCULATE YAW AND PITCH
	m_Camera.m_Yaw += (x - m_LastX) * m_Camera.m_Sensitivity;
	m_Camera.m_Pitch -= (y - m_LastY) * m_Camera.m_Sensitivity;

	//-89 <= PITCH <= 89, -180 < YAW < 180
	m_Camera.m_Pitch = std::clamp(m_Camera.m_Pitch, -89.0f, 89.0f);

	if (m_Camera.m_Yaw > 180.0f) {
		m_Camera.m_Yaw = m_Camera.m_Yaw - 360.0f;
	}
	if (m_Camera.m_Yaw < -180.0f) {
		m_Camera.m_Yaw = m_Camera.m_Yaw + 360.0f;
	}
	
	m_LastX = x;
	m_LastY = y;

	glm::vec3 direction;
	direction.z = sin(glm::radians(m_Camera.m_Yaw)) * cos(glm::radians(m_Camera.m_Pitch)); 
	direction.y = sin(glm::radians(m_Camera.m_Pitch)); 
	direction.x = cos(glm::radians(m_Camera.m_Yaw)) * cos(glm::radians(m_Camera.m_Pitch)); 
	m_Camera.m_Direction = glm::normalize(direction); 
}
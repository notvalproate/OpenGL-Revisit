#include "CameraHandler.hpp"
#include <iostream>
#include <algorithm>

CameraHandler::CameraHandler(Camera& p_Camera) : m_Cam(p_Camera), m_FirstMouse(true) {
	//Initializing default keybinds
	m_KeyBindings[KeyBind::FORWARD] = GLFW_KEY_W; 
	m_KeyBindings[KeyBind::BACKWARD] = GLFW_KEY_S;
	m_KeyBindings[KeyBind::LEFT] = GLFW_KEY_A; 
	m_KeyBindings[KeyBind::RIGHT] = GLFW_KEY_D; 
	m_KeyBindings[KeyBind::UP] = GLFW_KEY_SPACE; 
	m_KeyBindings[KeyBind::DOWN] = GLFW_KEY_LEFT_SHIFT;
	m_KeyBindings[KeyBind::SPRINT] = GLFW_KEY_LEFT_CONTROL;
}

void CameraHandler::SetKeybind(KeyBind p_Control, unsigned int p_Key) {
	m_KeyBindings[p_Control] = p_Key;
}

void CameraHandler::HandleEvents(GLFWwindow* p_Window, const float p_DeltaTime) {
	HandleKeyboardInput(p_Window, p_DeltaTime);
	HandleMouseInput(p_Window);
}

void CameraHandler::HandleKeyboardInput(GLFWwindow* p_Window, const float p_DeltaTime) {
	const auto f_Pressed = [&p_Window](unsigned int p_Key) {
		return glfwGetKey(p_Window, p_Key) == GLFW_PRESS;
	};

	float t_Mul = 1.0f;
	if (f_Pressed(m_KeyBindings[KeyBind::SPRINT])) {
		t_Mul = m_Cam.m_SpeedMultiplier;
	}
	
	//WASD
	if (f_Pressed(m_KeyBindings[KeyBind::FORWARD])) {
		m_Cam.m_Position += m_Cam.m_Direction * m_Cam.m_Speed * t_Mul * p_DeltaTime;
	}
	if (f_Pressed(m_KeyBindings[KeyBind::BACKWARD])) {
		m_Cam.m_Position -= m_Cam.m_Direction * m_Cam.m_Speed * t_Mul * p_DeltaTime;
	}
	if (f_Pressed(m_KeyBindings[KeyBind::LEFT])) {           //RIGHT VECTOR
		m_Cam.m_Position -= glm::normalize(glm::cross(m_Cam.m_Direction, m_Cam.m_Up)) * m_Cam.m_Speed * t_Mul * p_DeltaTime;
	}
	if (f_Pressed(m_KeyBindings[KeyBind::RIGHT])) {
		m_Cam.m_Position += glm::normalize(glm::cross(m_Cam.m_Direction, m_Cam.m_Up)) * m_Cam.m_Speed * t_Mul * p_DeltaTime;
	}

	//SPACE AND SHIFT FOR UP AND DOWN
	if (f_Pressed(m_KeyBindings[KeyBind::UP])) { 
		m_Cam.m_Position += glm::vec3(0.0f, 1.0f, 0.0f) * m_Cam.m_Speed * t_Mul * p_DeltaTime;
	}
	if (f_Pressed(m_KeyBindings[KeyBind::DOWN])) { 
		m_Cam.m_Position -= glm::vec3(0.0f, 1.0f, 0.0f) * m_Cam.m_Speed * t_Mul * p_DeltaTime;
	}
}

void CameraHandler::HandleMouseInput(GLFWwindow* p_Window) {
	double t_X, t_Y; 
	glfwGetCursorPos(p_Window, &t_X, &t_Y); 

	if (m_FirstMouse) {
		m_LastPosX = t_X;
		m_LastPosY = t_Y; 
		m_FirstMouse = false;
	}

	//CALCULATE YAW AND PITCH
	m_Cam.m_Yaw += (t_X - m_LastPosX) * m_Cam.m_Sensitivity;
	m_Cam.m_Pitch -= (t_Y - m_LastPosY) * m_Cam.m_Sensitivity;

	//-89 <= PITCH <= 89, -180 < YAW < 180
	m_Cam.m_Pitch = std::clamp(m_Cam.m_Pitch, -89.0, 89.0);

	if (m_Cam.m_Yaw > 180.0f) {
		m_Cam.m_Yaw = m_Cam.m_Yaw - 360.0f;
	}
	if (m_Cam.m_Yaw < -180.0f) {
		m_Cam.m_Yaw = m_Cam.m_Yaw + 360.0f;
	}
	
	m_LastPosX = t_X;
	m_LastPosY = t_Y;

	glm::vec3 t_Dir;
	t_Dir.z = sin(glm::radians(m_Cam.m_Yaw)) * cos(glm::radians(m_Cam.m_Pitch)); 
	t_Dir.y = sin(glm::radians(m_Cam.m_Pitch)); 
	t_Dir.x = cos(glm::radians(m_Cam.m_Yaw)) * cos(glm::radians(m_Cam.m_Pitch)); 
	m_Cam.m_Direction = glm::normalize(t_Dir); 
}
#pragma once
#include "Camera.hpp"
#include "GLFW/glfw3.h"
#include <unordered_map>

enum class KeyBind : unsigned short {
	FORWARD = 0x00,
	BACKWARD = 0x01,
	LEFT = 0x02,
	RIGHT = 0x03,
	UP = 0x04,
	DOWN = 0x05,
	SPRINT = 0x06
};

class CameraHandler {
public:
	CameraHandler(Camera& p_Camera);

	void HandleEvents(GLFWwindow* p_Window, const float p_DeltaTime);
	void SetKeybind(KeyBind p_Control, unsigned int p_Key);
private:
	Camera& m_Cam;	
	std::unordered_map<KeyBind, unsigned int> m_KeyBindings;
	double m_LastPosX, m_LastPosY;
	bool m_FirstMouse;

	void HandleKeyboardInput(GLFWwindow* p_Window, const float p_DeltaTime);
	void HandleMouseInput(GLFWwindow* p_Window);
};
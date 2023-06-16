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
	CameraHandler(Camera& a_Camera);

	void handleEvents(GLFWwindow* a_Window, const float a_DeltaTime);
	void setKeybind(KeyBind a_Control, unsigned int a_Key);
private:
	Camera& m_Camera;	
	std::unordered_map<KeyBind, unsigned int> m_KeyBindings;
	double m_LastX{}, m_LastY{};
	bool m_FirstMouse;

	void handleKeyboardInput(GLFWwindow* a_Window, const float a_DeltaTime) const;
	void handleMouseInput(GLFWwindow* a_Window);
};
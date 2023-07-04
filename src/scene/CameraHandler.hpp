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
	CameraHandler(Camera& camera);

	void handleEvents(GLFWwindow* window, const float deltaTime);
	void setKeybind(KeyBind control, unsigned int key);
private:
	Camera& m_Camera;	
	std::unordered_map<KeyBind, unsigned int> m_KeyBindings;
	float m_LastX{}, m_LastY{};
	bool m_FirstMouse;

	void handleKeyboardInput(GLFWwindow* window, const float deltaTime) const;
	void handleMouseInput(GLFWwindow* window);
};
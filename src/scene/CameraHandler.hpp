#pragma once
#include "Camera.hpp"
#include "GLFW/glfw3.h"

class CameraHandler {
public:
	CameraHandler(Camera& p_Camera) : m_Camera(p_Camera), m_FirstMouse(true) {}

	void HandleEvents(GLFWwindow* p_Window, const float p_DeltaTime);
	void HandleKeyboard(GLFWwindow* p_Window, const float p_DeltaTime);
	void HandleMouse(GLFWwindow* p_Window);
private:
	struct MousePos {
		double x{};
		double y{};
	};

	Camera& m_Camera;
	MousePos m_MousePos;
	bool m_FirstMouse;
};
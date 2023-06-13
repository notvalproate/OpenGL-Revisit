#pragma once
#include "Camera.hpp"
#include "GLFW/glfw3.h"

class CameraHandler {
public:
	CameraHandler(Camera& p_Camera) : m_Camera(p_Camera) {}

	void HandleEvents(GLFWwindow* p_Window, const float p_DeltaTime);
private:
	Camera& m_Camera;
};
#pragma once
#include <GLFW/glfw3.h>

class Timer {
public:
	void setStartTime(const float a_Time);
	[[nodiscard]] float getDeltaTime();
private:
	float m_CurrentFrame{};
	float m_LastFrame{};
	float m_DeltaTime{};
};
#pragma once
#include <GLFW/glfw3.h>

class Timer {
public:
	void SetStartTime(const float p_Time);
	[[nodiscard]] float GetDeltaTime();
private:
	float m_CurrentFrame{};
	float m_LastFrame{};
	float m_DeltaTime{};
};
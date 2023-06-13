#pragma once
#include <GLFW/glfw3.h>

class Timer {
public:
	void SetStartTime(const double p_Time);
	[[nodiscard]] double GetDeltaTime();
private:
	double m_CurrentFrame{};
	double m_LastFrame{};
	double m_DeltaTime{};
};
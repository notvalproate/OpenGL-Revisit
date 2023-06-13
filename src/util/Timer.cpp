#include "Timer.hpp"
#include <iostream>
void Timer::SetStartTime(const float p_Time) {
	glfwSetTime(p_Time);
	m_LastFrame = p_Time;
}

[[nodiscard]]
float Timer::GetDeltaTime() {
	m_CurrentFrame = glfwGetTime();
	m_DeltaTime = m_CurrentFrame - m_LastFrame;
	m_LastFrame = m_CurrentFrame;
	return m_DeltaTime;
}
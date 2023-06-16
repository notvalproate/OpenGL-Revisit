#include "Timer.hpp"
#include <iostream>

void Timer::setStartTime(const float a_Time) {
	glfwSetTime(a_Time);
	m_LastFrame = a_Time;
}

[[nodiscard]]
float Timer::getDeltaTime() {
	m_CurrentFrame = glfwGetTime();
	m_DeltaTime = m_CurrentFrame - m_LastFrame;
	m_LastFrame = m_CurrentFrame;
	return m_DeltaTime;
}
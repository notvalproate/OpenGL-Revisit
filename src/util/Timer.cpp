#include "Timer.hpp"
#include <iostream>

void Timer::setStartTime(const float time) {
	glfwSetTime(time);
	m_LastFrame = time;
}

[[nodiscard]]
float Timer::getDeltaTime() {
	m_CurrentFrame = glfwGetTime();
	m_DeltaTime = m_CurrentFrame - m_LastFrame;
	m_LastFrame = m_CurrentFrame;
	return m_DeltaTime;
}
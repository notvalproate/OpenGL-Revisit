#include "Timer.hpp"
#include <iostream>
void Timer::SetStartTime(const double p_Time) {
	glfwSetTime(p_Time);
	m_LastFrame = p_Time;
}

[[nodiscard]]
double Timer::GetDeltaTime() {
	m_CurrentFrame = glfwGetTime();
	m_DeltaTime = m_CurrentFrame - m_LastFrame;
	std::cout << m_CurrentFrame << " " << m_LastFrame << " " << m_DeltaTime << std::endl;
	m_LastFrame = m_CurrentFrame;
	return m_DeltaTime;
}
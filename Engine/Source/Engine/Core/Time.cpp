#include "Time.h"

namespace sl
{

void Clock::Tick()
{
	auto crtTimePoint = std::chrono::high_resolution_clock::now();
	long long deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(crtTimePoint - m_lastTimePoint).count();
	m_deltaTime = (float)deltaTime / 1000.0f;
	m_lastTimePoint = crtTimePoint;
}

Timer::Timer() : m_startTimePoint(std::chrono::high_resolution_clock::now())
{

}

void Timer::Reset()
{
	m_startTimePoint = std::chrono::high_resolution_clock::now();
}

float Timer::GetDuration()
{
	auto crtTimePoint = std::chrono::high_resolution_clock::now();
	long long deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(crtTimePoint - m_startTimePoint).count();
	return (float)deltaTime / 1000.0f;
}

} // namespace sl

#include "Timer.h"

namespace sl
{

void Timer::Update()
{
	auto crtTimePoint = std::chrono::high_resolution_clock::now();
	long long deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(crtTimePoint - m_lastTimePoint).count();
	m_deltaTime = (float)deltaTime / 1000000.0f;
	m_lastTimePoint = crtTimePoint;
}

} // namespace sl

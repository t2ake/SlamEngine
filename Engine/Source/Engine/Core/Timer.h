#pragma once

#include <chrono>

namespace sl
{

class Timer final
{
public:
	Timer() = default;

	void Update();
	float GetDeltatIme() const { return m_deltaTime; }

private:
	std::chrono::steady_clock::time_point m_lastTimePoint;
	float m_deltaTime = 0.0f;
};

} // namespace sl

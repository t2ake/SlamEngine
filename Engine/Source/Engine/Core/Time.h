#pragma once

#include "Core/Core.h"

#include <chrono>

namespace sl
{

class Clock final
{
public:
	Clock() = default;

	void Tick();

	// Returns in milliseconds.
	float GetDeltatIme() const { return m_deltaTime; }

private:
	std::chrono::steady_clock::time_point m_lastTimePoint;
	float m_deltaTime = 0.0f;
};

class Timer final
{
public:
	Timer();

	void Reset();

	// Returns in milliseconds.
	float GetDuration();

private:
	std::chrono::steady_clock::time_point m_startTimePoint;
};

} // namespace sl

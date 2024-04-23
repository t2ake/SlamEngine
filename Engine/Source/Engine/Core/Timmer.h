#pragma once

#include <chrono>

namespace sl
{

class Timmer final
{
public:
	void Update();
	float GetDeltatIme() const { return m_deltaTime; }

private:
	float m_lastTime = 0.0f;
	float m_deltaTime = 0.0f;
};

} // namespace sl

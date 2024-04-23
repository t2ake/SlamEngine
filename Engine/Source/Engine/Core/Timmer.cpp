#include "Timmer.h"

// TEMPORARY
#include <glfw/glfw3.h>

namespace sl
{

void Timmer::Update()
{
	float crtTime = (float)glfwGetTime();
	m_deltaTime = crtTime - m_lastTime;
	m_lastTime = crtTime;
}

} // namespace sl

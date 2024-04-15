#include "Input.h"

#include "Window/Window.h"

#include <GLFW/glfw3.h>

namespace sl
{

Window *Input::m_pWindow = nullptr;

bool Input::IsKeyPressed(int key)
{
	GLFWwindow *pWindow = m_pWindow->GetWindow();
	auto state = glfwGetKey(pWindow, key);
	return GLFW_PRESS == state || GLFW_REPEAT == state;
}

bool Input::IsMouseButtonPressed(int button)
{
	GLFWwindow *pWindow = m_pWindow->GetWindow();
	auto state = glfwGetMouseButton(pWindow, button);
	return GLFW_PRESS == state || GLFW_REPEAT == state;
}

std::pair<float, float> Input::GetMousePos()
{
	GLFWwindow *pWindow = m_pWindow->GetWindow();
	double posX, posY;
	glfwGetCursorPos(pWindow, &posX, &posY);
	return { float(posX), float(posY)};
}

} // namespace sl

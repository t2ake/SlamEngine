#include "Input.h"

#include "Window/Window.h"

#include <GLFW/glfw3.h>

namespace sl
{

void *Input::m_pWindow = nullptr;

bool Input::IsKeyPressed(int key)
{
	auto state = glfwGetKey(static_cast<GLFWwindow *>(m_pWindow), key);
	return GLFW_PRESS == state;
}

bool Input::IsMouseButtonPressed(int button)
{
	auto state = glfwGetMouseButton(static_cast<GLFWwindow *>(m_pWindow), button);
	return GLFW_PRESS == state;
}

glm::vec2 Input::GetMousePos()
{
	double posX, posY;
	glfwGetCursorPos(static_cast<GLFWwindow *>(m_pWindow), &posX, &posY);
	return { float(posX), float(posY)};
}

} // namespace sl

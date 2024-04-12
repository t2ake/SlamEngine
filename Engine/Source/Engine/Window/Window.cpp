#include "Window.h"

#include "Log/Log.h"

#include <GLFW/glfw3.h>

namespace sl
{

Window::Window(std::string title, uint32_t width, uint32_t height, bool VSync)
	: m_title(title), m_width(width), m_height(height), m_isVSync(VSync)
{
	Init();
}

Window::~Window()
{
	Shutdown();
}

void Window::Init()
{
	SL_ENGINE_INFO("Creating window \"{}\" ({}, {})", m_title, m_width, m_height);

	glfwInit();
	m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_pWindow);
	glfwSwapInterval(m_isVSync);
}

void Window::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_pWindow);
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_pWindow);
}

} // namespace sl

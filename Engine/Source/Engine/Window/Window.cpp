#include "Window.h"

#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Log/Log.h"

#include <glad/glad.h>
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));

	glfwSwapInterval(m_isVSync);
	glfwSetWindowUserPointer(m_pWindow, this);

	// Callbacks
	glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow *window, int width, int height)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		pWindow->SetWidth(uint32_t(width));
		pWindow->SetHeight(uint32_t(height));

		WindowResizeEvent event{ uint32_t(width), uint32_t(height) };
		pWindow->DespatchEvent(event);
	});

	glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow *window)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		WindowCloseEvent event{};
		pWindow->DespatchEvent(event);
	});

	glfwSetKeyCallback(m_pWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent event{ key , false};
				pWindow->DespatchEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event{ key };
				pWindow->DespatchEvent(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event{ key , true };
				pWindow->DespatchEvent(event);
				break;
			}
			default:
			{
				SL_ENGINE_ASSERT(false);
			}
		}
	});

	glfwSetCharCallback(m_pWindow, [](GLFWwindow *window, unsigned int codepoint)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		KeyTypedEvent event{ int(codepoint) };
		pWindow->DespatchEvent(event);
	});

	glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow *window, int button, int action, int mods)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event{ uint32_t(button) };
				pWindow->DespatchEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event{ uint32_t(button) };
				pWindow->DespatchEvent(event);
				break;
			}
			default:
			{
				SL_ENGINE_ASSERT(false);
			}
		}
	});

	glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow *window, double xpos, double ypos)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		MouseMovedEvent event{ float(xpos), float(ypos) };
		pWindow->DespatchEvent(event);
	});

	glfwSetScrollCallback(m_pWindow, [](GLFWwindow *window, double xoffset, double yoffset)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		MouseScrolledEvent event{ float(xoffset), float(yoffset) };
		pWindow->DespatchEvent(event);
	});

	glfwSetErrorCallback([](int error_code, const char *description)
	{
		SL_ENGINE_ERROR("GLFW error {}: {}", error_code, description);
	});
}

void Window::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_pWindow);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

} // namespace sl

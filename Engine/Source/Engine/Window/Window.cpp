#include "Window.h"

#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

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
	SL_ENGINE_INFO("Create window \"{}\" ({}, {})", m_title, m_width, m_height);

	bool success = glfwInit();
	SL_ENGINE_ASSERT_INFO(success, "GLFW init failed!");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	SL_ENGINE_ASSERT_INFO(m_pWindow, "GLFW creat window failed!");
	m_pRenderContext = RenderContext::Create(m_pWindow);

	glfwSetWindowUserPointer(m_pWindow, this);
#ifdef SL_FINAL
	m_isVSync = false;
#endif
	glfwSwapInterval(m_isVSync ? 1 : 0);

	SetCallbacks();
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();

	delete m_pRenderContext;
}

void Window::EndFrame()
{
	m_pRenderContext->Bind();
	m_pRenderContext->SwapBuffers();
	glfwPollEvents();
}

void Window::SetVSync(bool VSync)
{
#ifndef SL_FINAL
	m_isVSync = VSync;
	glfwSwapInterval(m_isVSync ? 1 : 0);
#endif
}

void Window::DisableCursor() const
{
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::EnableCursor() const
{
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::SetCallbacks()
{
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
				KeyPressedEvent event{ key , false };
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
				MouseButtonPressedEvent event{ button };
				pWindow->DespatchEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event{ button };
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

	glfwSetWindowFocusCallback(m_pWindow, [](GLFWwindow *window, int focused)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if (GLFW_TRUE == focused)
		{
			WindowFocusEvent event;
			pWindow->DespatchEvent(event);
		}
		if(GLFW_FALSE == focused)
		{
			WindowLostFocusEvent event;
			pWindow->DespatchEvent(event);
		}
	});

	glfwSetDropCallback(m_pWindow, [](GLFWwindow *window, int path_count, const char *paths[])
	{
		if (path_count > 1)
		{
			SL_ENGINE_ERROR("Only support droping one file at a time.");
			return;
		}
		SL_ENGINE_ASSERT(1 == path_count);

		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		WindowDropEvent event{ paths[0] };
		pWindow->DespatchEvent(event);
	});

	glfwSetErrorCallback([](int error_code, const char *description)
	{
		SL_ENGINE_ERROR("GLFW error {}: {}", error_code, description);
	});
}

} // namespace sl

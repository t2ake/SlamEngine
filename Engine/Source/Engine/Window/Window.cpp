#include "Window.h"

#include "Core/Defines.h"
#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "RenderCore/RenderContext.h"
#include "RenderCore/RenderCore.h"

#include <GLFW/glfw3.h>

namespace sl
{

namespace
{

void SetCallbacks(GLFWwindow *pNativeWindow)
{
	glfwSetWindowSizeCallback(pNativeWindow, [](GLFWwindow *window, int width, int height)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		WindowResizeEvent event{ uint32_t(width), uint32_t(height) };
		pWindow->DespatchEvent(event);
	});

	glfwSetWindowCloseCallback(pNativeWindow, [](GLFWwindow *window)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		WindowCloseEvent event{};
		pWindow->DespatchEvent(event);
	});

	glfwSetKeyCallback(pNativeWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressEvent event{ key , false };
				pWindow->DespatchEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleaseEvent event{ key };
				pWindow->DespatchEvent(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressEvent event{ key , true };
				pWindow->DespatchEvent(event);
				break;
			}
			default:
			{
				SL_ASSERT(false);
			}
		}
	});

	glfwSetCharCallback(pNativeWindow, [](GLFWwindow *window, unsigned int codepoint)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		KeyTypeEvent event{ int(codepoint) };
		pWindow->DespatchEvent(event);
	});

	glfwSetMouseButtonCallback(pNativeWindow, [](GLFWwindow *window, int button, int action, int mods)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));

		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressEvent event{ button };
				pWindow->DespatchEvent(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleaseEvent event{ button };
				pWindow->DespatchEvent(event);
				break;
			}
			default:
			{
				SL_ASSERT(false);
			}
		}
	});

	glfwSetCursorPosCallback(pNativeWindow, [](GLFWwindow *window, double xpos, double ypos)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		MouseMoveEvent event{ float(xpos), float(ypos) };
		pWindow->DespatchEvent(event);
	});

	glfwSetScrollCallback(pNativeWindow, [](GLFWwindow *window, double xoffset, double yoffset)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		MouseScrollEvent event{ float(xoffset), float(yoffset) };
		pWindow->DespatchEvent(event);
	});

	glfwSetWindowFocusCallback(pNativeWindow, [](GLFWwindow *window, int focused)
	{
		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if (focused == GLFW_TRUE)
		{
			WindowGetFocusEvent event;
			pWindow->DespatchEvent(event);
		}
		if (focused == GLFW_FALSE)
		{
			WindowLostFocusEvent event;
			pWindow->DespatchEvent(event);
		}
	});

	glfwSetDropCallback(pNativeWindow, [](GLFWwindow *window, int path_count, const char *paths[])
	{
		if (path_count > 1)
		{
			SL_LOG_ERROR("Only one file can be dropped simultaneously for now!");
			return;
		}

		Window *pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		WindowDropEvent event{ paths[0] };
		pWindow->DespatchEvent(event);
	});

	glfwSetErrorCallback([](int error_code, const char *description)
	{
		SL_LOG_ERROR("GLFW error {}: {}", error_code, description);
	});
}

} // namespace

Window::Window()
{

}

Window::~Window()
{

}

void Window::Init(std::string_view title, uint32_t width, uint32_t height)
{
	SL_LOG_INFO("Create window \"{}\" ({}, {})", title, width, height);

// Init GLFW
	bool initSuccess = glfwInit();
	SL_ASSERT(initSuccess, "GLFW initialization failed!");

	// Window hints by graphics backend
	if (RenderCore::GetBackend() == GraphicsBackend::OpenGL)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	else
	{
		SL_ASSERT(false, "Slam only support OpenGL for now!");
	}

	// Creat window
	m_pNativeWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
	SL_ASSERT(m_pNativeWindow, "GLFW creating window failed!");
	m_pRenderContext.reset(RenderContext::Create(m_pNativeWindow));

	// Other settings
	glfwMaximizeWindow(static_cast<GLFWwindow *>(m_pNativeWindow));
#if defined(SL_FINAL)
	glfwSwapInterval(0);
#else
	glfwSwapInterval(1);
#endif

	// Callbacks
	glfwSetWindowUserPointer(static_cast<GLFWwindow *>(m_pNativeWindow), this);
	SetCallbacks(static_cast<GLFWwindow *>(m_pNativeWindow));
}

void Window::Terminate()
{
	glfwDestroyWindow(static_cast<GLFWwindow *>(m_pNativeWindow));
	glfwTerminate();
}

void Window::BegineFrame()
{

}

void Window::EndFrame()
{
	m_pRenderContext->MakeCurrent();
	m_pRenderContext->SwapBuffers();
	glfwPollEvents();
}

void Window::SetVSync(bool VSync)
{
	glfwSwapInterval(VSync ? 1 : 0);
}

void Window::CursorModeDisabled()
{
	glfwSetInputMode(static_cast<GLFWwindow *>(m_pNativeWindow), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::CursorModeNormal()
{
	glfwSetInputMode(static_cast<GLFWwindow *>(m_pNativeWindow), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::SetCursorPos(float x, float y)
{
	glfwSetCursorPos(static_cast<GLFWwindow *>(m_pNativeWindow), (double)x, (double)y);
}

void Window::SetGlobalCursorPos(float x, float y)
{
	int windowPosX, windowPosY;
	glfwGetWindowPos(static_cast<GLFWwindow *>(m_pNativeWindow), &windowPosX, &windowPosY);
	glfwSetCursorPos(static_cast<GLFWwindow *>(m_pNativeWindow), (double)x - (double)windowPosX, (double)y - (double)windowPosY);
}

} // namespace sl

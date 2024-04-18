#include "Window.h"

#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Log/Log.h"
#include "Platform/OpenGL/OpenGLContext.h"

// TEMPORARY
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

	bool success = glfwInit();
	SL_ENGINE_ASSERT_INFO(success, "GLFW init failed!");

	m_pRenderingContext = new OpenGLContext;
	m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	SL_ENGINE_ASSERT_INFO(m_pWindow, "GLFW creat window failed!");
	m_pRenderingContext->Init(m_pWindow);

	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSwapInterval(m_isVSync ? 1 : 0);
	SetCallbacks();

	// TEMPORARY
	{
		uint32_t m_vertexArray, m_vertexBuffer, m_indexBuffer;

		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);

		glGenBuffers(1, &m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		constexpr float vertices[9] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
		constexpr uint32_t indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vsSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_position;
			out vec3 v_position;

			void main()
			{
				v_position = a_position;
				gl_Position = vec4(a_position, 1.0);
			}
		)";

		std::string fsSrc = R"(
			#version 330 core
			in vec3 v_position;
			out vec4 color;

			void main()
			{
				color = vec4(v_position * 0.5 + 0.5, 1.0);
			}
		)";

		m_pShader = new Shader{ std::move(vsSrc), std::move(fsSrc) };
	}
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();

	delete m_pRenderingContext;
}

void Window::BegineFrame()
{
	// TEMPORARY
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Update()
{
	// TEMPORARY
	m_pShader->Bind();
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}

void Window::Render()
{

}

void Window::EndFrame()
{
	glfwMakeContextCurrent(m_pWindow);

	m_pRenderingContext->SwapBuffers();
	glfwPollEvents();
}

void Window::SetVSync(bool VSync)
{
	m_isVSync = VSync;
	glfwSwapInterval(m_isVSync ? 1 : 0);
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

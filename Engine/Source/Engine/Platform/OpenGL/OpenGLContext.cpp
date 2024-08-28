#include "OpenGLContext.h"

#include "Core/Log.h"
#include "Utils/ProfilerCPU.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace sl
{

OpenGLContext::OpenGLContext(void *pWindow) : m_pWindow(pWindow)
{
	SL_PROFILE;

	glfwMakeContextCurrent(static_cast<GLFWwindow *>(m_pWindow));

	bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	SL_ASSERT(success, "Failed to initialize GLAD context!");

	SL_LOG_INFO("OpenGL inof:");
	SL_LOG_INFO("  Vendor: {}", (const char *)glGetString(GL_VENDOR));
	SL_LOG_INFO("  Renderer: {}", (const char *)glGetString(GL_RENDERER));
	SL_LOG_INFO("  Version: {}", (const char *)glGetString(GL_VERSION));
}

void OpenGLContext::MakeCurrent()
{
	SL_PROFILE;

	glfwMakeContextCurrent(static_cast<GLFWwindow *>(m_pWindow));
}

void OpenGLContext::SwapBuffers()
{
	SL_PROFILE;

	glfwSwapBuffers(static_cast<GLFWwindow *>(m_pWindow));
}

} // namespace sl

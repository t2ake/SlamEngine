#include "OpenGLContext.h"

#include "Core/Log.h"
#include "Utils/ProfilerCPU.h"

#include <glad/glad.h>
#include <SDL2/SDL.h>

namespace sl
{

OpenGLContext::OpenGLContext(void *pWindow) : m_pWindow(pWindow)
{
	SL_PROFILE;

	m_pContext = SDL_GL_CreateContext(static_cast<SDL_Window *>(m_pWindow));
	SDL_GL_MakeCurrent(static_cast<SDL_Window *>(m_pWindow), m_pContext);

	bool success = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	SL_ASSERT(success, "Failed to initialize GLAD context!");

	SL_LOG_INFO("OpenGL inof:");
	SL_LOG_INFO("\tVendor: {}", (const char *)glGetString(GL_VENDOR));
	SL_LOG_INFO("\tRenderer: {}", (const char *)glGetString(GL_RENDERER));
	SL_LOG_INFO("\tVersion: {}", (const char *)glGetString(GL_VERSION));
}

OpenGLContext::~OpenGLContext()
{
	SDL_GL_DeleteContext(m_pContext);
}

void OpenGLContext::MakeCurrent()
{
	SDL_GL_MakeCurrent(static_cast<SDL_Window *>(m_pWindow), m_pContext);
}

void OpenGLContext::SwapBuffers()
{
	SDL_GL_SwapWindow(static_cast<SDL_Window *>(m_pWindow));
}

} // namespace sl

static_assert(std::is_same_v<SDL_GLContext, void *>); // For void *m_pContext

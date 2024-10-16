#include "Window.h"

#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "RenderCore/RenderContext.h"
#include "RenderCore/RenderCore.h"
#include "Utils/ProfilerCPU.h"
#include "Core/Time.hpp"

#include <SDL2/SDL.h>

namespace sl
{

Window::Window()
{

}

Window::~Window()
{

}

void Window::Init(std::string_view title, uint32_t width, uint32_t height)
{
	SL_PROFILE;
	SL_LOG_INFO("Create window \"{}\" ({}, {})", title, width, height);

	// Init SDL.
	int initSuccess = SDL_Init(SDL_INIT_VIDEO);
	SL_ASSERT(initSuccess == 0, "Failed to initialize SDL:\n\t{}", SDL_GetError());
	// SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_CURSOR_VISIBLE, "1");
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0");
	SDL_SetHint(SDL_HINT_WINDOWS_DPI_SCALING, "1");

	// Creat window.
	uint32_t windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_ALLOW_HIGHDPI;
	switch (RenderCore::GetBackend())
	{
		case GraphicsBackend::OpenGL:
		{
			windowFlags |= SDL_WINDOW_OPENGL;

			// OpenGL 4.5 core profile.
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
			break;
		}
		default:
		{
			SL_ASSERT(false, "Only support OpenGL backend for now!");
		}
	}

	m_pNativeWindow = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
	SL_ASSERT(m_pNativeWindow, "Failed to create SDL window:\n\t{}", SDL_GetError());

	// Create rendering context.
	m_pRenderContext.reset(RenderContext::Create(m_pNativeWindow));

	// Other settings.
	if (SDL_GL_SetSwapInterval(-1) < 0)
	{
		SL_LOG_WARN("Failed to enable adaptive vsync, enabled normal vsync instead.\n\t{}", SDL_GetError());
		SDL_GL_SetSwapInterval(1);
	}
}

void Window::Terminate()
{
	SL_PROFILE;

	m_pRenderContext.reset();
	SDL_DestroyWindow(static_cast<SDL_Window *>(m_pNativeWindow));
	SDL_Quit();
}

void Window::BegineFrame()
{

}

void Window::EndFrame()
{
	SL_PROFILE;

	m_pRenderContext->MakeCurrent();
	m_pRenderContext->SwapBuffers();

	PullEvents();
}

void *Window::GetRenderContext() const
{
	return m_pRenderContext->GetContext();
}

void Window::SetVSync(VSyncMode mode) const
{
	if (SDL_GL_SetSwapInterval(static_cast<int>(mode)) < 0)
	{
		SL_LOG_ERROR(SDL_GetError());
	}
}

glm::ivec2 Window::GetSize() const
{
	int sizeX, sizeY;
	SDL_GetWindowSize(static_cast<SDL_Window *>(m_pNativeWindow), &sizeX, &sizeY);
	return glm::ivec2{ sizeX, sizeY };
}

void Window::PullEvents()
{
	SL_PROFILE;

	SDL_Event SDLEvent;
	while (SDL_PollEvent(&SDLEvent))
	{
		ImGuiContext::OnEvent(&SDLEvent);

		switch (SDLEvent.type)
		{
			case SDL_WINDOWEVENT:
			{
				ForwardWindowEvents(SDLEvent.window);
				break;
			}
			case SDL_KEYDOWN:
			{
				KeyPressEvent event{ (int)SDLEvent.key.keysym.scancode, (bool)SDLEvent.key.repeat };
				m_eventCallback(event);
				break;
			}
			case SDL_KEYUP:
			{
				KeyReleaseEvent event{ (int)SDLEvent.key.keysym.scancode };
				m_eventCallback(event);
				break;
			}
			case SDL_MOUSEMOTION:
			{
				MouseMoveEvent event{ SDLEvent.motion.x, SDLEvent.motion.y };
				m_eventCallback(event);
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				MouseButtonPressEvent event{ (int)SDLEvent.button.button };
				m_eventCallback(event);
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				MouseButtonReleaseEvent  event{ (int)SDLEvent.button.button };
				m_eventCallback(event);
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				MouseScrollEvent event{ SDLEvent.wheel.preciseX, SDLEvent.wheel.preciseY };
				m_eventCallback(event);
				break;
			}

			// TODO: Drop Event
		}
	}
}

void Window::ForwardWindowEvents(SDL_WindowEvent &SDLWindowEvent)
{
	switch (SDLWindowEvent.event)
	{
		case SDL_WINDOWEVENT_CLOSE:
		{
			WindowCloseEvent event;
			m_eventCallback(event);
			break;
		}
		case SDL_WINDOWEVENT_RESIZED:
		{
			WindowResizeEvent event{ (uint32_t)SDLWindowEvent.data1, (uint32_t)SDLWindowEvent.data2 };
			m_eventCallback(event);
			break;
		}
		case SDL_WINDOWEVENT_MINIMIZED:
		{
			WindowMinimizeEvent event;
			m_eventCallback(event);
			break;
		}
		case SDL_WINDOWEVENT_MAXIMIZED:
		{
			WindowMaximizeEvent event;
			m_eventCallback(event);
			break;
		}
		case SDL_WINDOWEVENT_RESTORED:
		{
			WindowRestoreEvent event;
			m_eventCallback(event);
			break;
		}
		case SDL_WINDOWEVENT_FOCUS_GAINED:
		{
			WindowGetFocusEvent event;
			m_eventCallback(event);
			break;
		}
		case SDL_WINDOWEVENT_FOCUS_LOST:
		{
			WindowLossFocusEvent event;
			m_eventCallback(event);
			break;
		}
		case SDL_WINDOWEVENT_TAKE_FOCUS:
		{
			WindowLossFocusEvent event;
			m_eventCallback(event);
			break;
		}
		default:
		{
			//SL_LOG_DEBUG("Unknown window event: {}", nameof::nameof_enum(static_cast<SDL_WindowEventID>(SDLWindowEvent.event)));
		}
	}
}

} // namespace sl

#include "Input.h"

#include "Core/Log.h"
#include "ImGui/ImGuiContext.h"
#include "Window/Window.h"

#include <imgui/imgui.h>
#include <SDL2/SDL.h>

namespace sl
{

void Input::Init(void *pWindow)
{
	m_pWindow = pWindow;
}

bool Input::IsKeyPressed(int key)
{
	const auto *pStates = SDL_GetKeyboardState(nullptr);
	return pStates[static_cast<size_t>(key)];
}

bool Input::IsMouseButtonPressed(int button)
{
	uint32_t state = SDL_GetMouseState(nullptr, nullptr);
	return state & (1 << (button - 1));
}

glm::ivec2 Input::GetMousePos()
{
	int posX, posY;
	SDL_GetMouseState(&posX, &posY);
	return glm::ivec2{ posX, posY };
}

glm::ivec2 Input::GetMouseGlobalPos()
{
	int posX, posY;
	SDL_GetGlobalMouseState(&posX, &posY);
	return glm::ivec2{ posX, posY };
}

glm::ivec2 Input::GetMouseDelta()
{
	int deltaX, deltaY;
	SDL_GetRelativeMouseState(&deltaX, &deltaY);
	return glm::ivec2{ deltaX, deltaY };
}

void Input::SetMousePos(glm::ivec2 pos)
{
	SDL_WarpMouseInWindow(static_cast<SDL_Window *>(m_pWindow), pos.x, pos.y);
}

void Input::SetMouseGlobalPos(glm::ivec2 globalPos)
{
	SDL_WarpMouseGlobal(globalPos.x, globalPos.y);
}

void Input::SetMouseWrapMode(bool wrap, bool restoreMousePos)
{
	static int s_posX;
	static int s_posY;

	if (wrap)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		if (restoreMousePos)
		{
			SDL_GetMouseState(&s_posX, &s_posY);
		}
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		if (restoreMousePos)
		{
			SDL_WarpMouseInWindow(static_cast<SDL_Window *>(m_pWindow), s_posX, s_posY);
		}
	}
}

} // namespace sl

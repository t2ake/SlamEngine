#include "Input.h"

#include "Window/Window.h"

#include <SDL2/SDL.h>

namespace sl
{

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

glm::vec2 Input::GetMousePos()
{
	int posX, posY;
	SDL_GetMouseState(&posX, &posY);
	return glm::vec2{ float(posX), float(posY)};
}

glm::vec2 Input::GetMouseDelta()
{
	int deltaX, deltaY;
	SDL_GetRelativeMouseState(&deltaX, &deltaY);
	return glm::vec2{ float(deltaX), float(deltaY) };
}

} // namespace sl

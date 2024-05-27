#pragma once

#include "Window/KeyCode.h"
#include "Window/MouseButtonCode.h"

#include <glm/vec2.hpp>

namespace sl
{

class Input final
{
public:
	Input() = delete;

	static bool IsKeyPressed(int key);
	static bool IsMouseButtonPressed(int button);

	// Use the upper left corner of the window as the coordinate origin.
	static glm::vec2 GetMousePos();
	// Use the upper left corner of the monitor as the coordinate origin.
	static glm::vec2 GetGlobalMousePos();

	static void Init(void *pWindow) { m_pWindow = pWindow; }

	inline static void *m_pWindow = nullptr;
};

} // namespace sl

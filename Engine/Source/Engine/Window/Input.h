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
	static glm::vec2 GetMousePos();

	static void Init(void *pWindow) { m_pWindow = pWindow; }

	inline static void *m_pWindow = nullptr;
};

} // namespace sl

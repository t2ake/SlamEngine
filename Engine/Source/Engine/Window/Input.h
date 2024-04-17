#pragma once

#include "Window/KeyCode.h"
#include "Window/MouseButtonCode.h"

#include <glm/vec2.hpp>

namespace sl
{

class Window;

class Input final
{
private:
	Input() = default;

public:
	static Input &GetInstance()
	{
		static Input instance;
		return instance;
	}

	static void SetWindow(Window *pWindow) { m_pWindow = pWindow; }
	static Window *GetWindow() { return m_pWindow; }

	static bool IsKeyPressed(int key);
	static bool IsMouseButtonPressed(int button);
	static glm::vec2 GetMousePos();

private:
	static Window *m_pWindow;
};

} // namespace sl

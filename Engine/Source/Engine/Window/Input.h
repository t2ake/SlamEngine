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
	Input(const Input &) = delete;
	Input &operator=(const Input &) = delete;
	Input(Input &&) = delete;
	Input &operator=(Input &&) = delete;
	~Input() = delete;

	static void Init(void *pWindow) { m_pWindow = pWindow; }

	static bool IsKeyPressed(int key);
	static bool IsMouseButtonPressed(int button);
	
	// Use the upper left corner of the window as the coordinate origin.
	static glm::vec2 GetMousePos();
	static glm::vec2 GetMouseDelta();
	
private:
	inline static void *m_pWindow = nullptr;
};

} // namespace sl

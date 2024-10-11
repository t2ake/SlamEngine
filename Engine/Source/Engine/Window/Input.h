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

	static void Init(void *pWindow);

	static bool IsKeyPressed(int key);
	static bool IsMouseButtonPressed(int button);

	static glm::ivec2 GetMousePos();
	static glm::ivec2 GetMouseGlobalPos();
	static glm::ivec2 GetMouseDelta();

	static void SetMousePos(glm::ivec2 pos);
	static void SetMouseGlobalPos(glm::ivec2 globalPos);

	static void SetMouseWrapMode(bool wrap, bool restoreMousePos = true);

private:
	inline static void *m_pWindow = nullptr;
};

} // namespace sl

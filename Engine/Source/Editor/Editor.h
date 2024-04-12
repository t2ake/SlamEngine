#pragma once

#include <memory>

namespace sl
{

class Window;

}

class Editor
{
public:
	Editor() = default;
	Editor(const Editor &) = delete;
	Editor &operator=(const Editor &) = delete;
	Editor(Editor &&) = delete;
	Editor &operator=(Editor &&) = delete;

	~Editor();

	void Init();
	void Update();
	void Render();
	void Shutdown();

private:
	sl::Window *m_pWindow;
};

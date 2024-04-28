#pragma once

#include "Core/Defines.h"
#include "Core/Timer.h"

namespace sl
{

class Event;
class LayerStack;
class Window;
class WindowCloseEvent;
class WindowResizeEvent;

}

struct EditorInitor
{
	std::string title;
	uint32_t m_width;
	uint32_t m_height;
	sl::GraphicsBackend m_backend;
};

class Editor final
{
public:
	static Editor *Get() { return pInstance; }
	static Editor *pInstance;

public:
	Editor() = delete;
	Editor(const Editor &) = delete;
	Editor &operator=(const Editor &) = delete;
	Editor(Editor &&) = delete;
	Editor &operator=(Editor &&) = delete;

	Editor(EditorInitor initor);
	~Editor();

	void Run();

	sl::Window *GetWindow() const { return m_pWindow; }

private:
	void BegineFrame();
	void Render();
	void EndFrame();

	void OnEvent(sl::Event &event);
	bool OnWindowClose(sl::WindowCloseEvent &event);
	bool OnWindowResized(sl::WindowResizeEvent &event);

	bool m_isRunning = true;
	bool m_isMinimized = false;

	sl::Timer m_timer;
	
	sl::LayerStack *m_pLayerStack = nullptr;
	sl::Window *m_pWindow = nullptr;
};

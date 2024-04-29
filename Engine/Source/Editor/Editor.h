#pragma once

#include "Core/Defines.h"
#include "Core/Timer.h"

namespace sl
{

class Event;
class WindowCloseEvent;
class WindowResizeEvent;
class MouseButtonReleasedEvent;
class SceneViewportFocusEvent;
class Window;
class LayerStack;

}

class SandboxLayer;
class ImGuiLayer;

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
	bool OnWindowResize(sl::WindowResizeEvent &event);
	bool OnMouseButtonReleased(sl::MouseButtonReleasedEvent &event);

	bool m_isRunning = true;
	bool m_isMinimized = false;

	sl::Timer m_timer;
	sl::Window *m_pWindow = nullptr;
	sl::LayerStack *m_pLayerStack = nullptr;
	SandboxLayer *m_pSandboxLayer = nullptr;
	ImGuiLayer *m_pImGuiLayer = nullptr;
};

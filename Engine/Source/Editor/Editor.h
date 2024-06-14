#pragma once

#include "Core/Core.h"
#include "Core/Defines.h"
#include "Core/Timer.h"

namespace sl
{

class Event;
class WindowCloseEvent;
class WindowResizeEvent;
class LayerStack;

}

class CameraControllerLayer;
class ImGuiLayer;
class RendererLayer;
class SandboxLayer;
class WindowLayer;

struct EditorInitor
{
	std::string_view title;
	uint32_t m_width;
	uint32_t m_height;
	sl::GraphicsBackend m_backend;
};

class Editor final
{
public:
	Editor() = delete;
	Editor(const Editor &) = delete;
	Editor &operator=(const Editor &) = delete;
	Editor(Editor &&) = delete;
	Editor &operator=(Editor &&) = delete;

	Editor(EditorInitor initor);
	~Editor();

	void Run();

private:
	void BegineFrame();
	void Update();
	void Render();
	void EndFrame();

	void OnEvent(sl::Event &event);
	bool OnWindowClose(sl::WindowCloseEvent &event);
	bool OnWindowResize(sl::WindowResizeEvent &event);

	bool m_isRunning = true;
	bool m_isMinimized = false;

	sl::Timer m_timer;
	sl::LayerStack *m_pLayerStack = nullptr;

	CameraControllerLayer *m_pCameraControllerLayer = nullptr;
	ImGuiLayer *m_pImGuiLayer = nullptr;
	RendererLayer *m_pRendererLayer = nullptr;
	SandboxLayer *m_pSandboxLayer = nullptr;
	WindowLayer *m_pWindowLayer = nullptr;
};

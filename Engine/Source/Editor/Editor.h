#pragma once

#include "Layer/LayerStack.h"

namespace sl
{

class Event;
class Window;

}

struct EditorInitor
{
	std::string title;
	uint32_t m_width;
	uint32_t m_height;
};

class Editor
{
public:
	Editor() = delete;
	Editor(const Editor &) = delete;
	Editor &operator=(const Editor &) = delete;
	Editor(Editor &&) = delete;
	Editor &operator=(Editor &&) = delete;

	Editor(EditorInitor initor);
	~Editor();

	void Update();

private:
	void Init(EditorInitor initor);
	void Shutdown();

	void Render();

	void OnEvent(sl::Event &e);
	bool OnWindowClose(sl::Event &e);

	void PushLayer(sl::Layer *pLayer);
	void PopLayer(sl::Layer *pLayer);

	bool m_isRunning = true;

	sl::Window *m_pWindow = nullptr;

	sl::LayerStack m_layerStack;
};

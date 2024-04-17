#pragma once

#include <cstdint>
#include <string>

namespace sl
{

class Event;
class LayerStack;
class Window;

}

struct EditorInitor
{
	std::string title;
	uint32_t m_width;
	uint32_t m_height;
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

	void Update();

private:
	void Init(EditorInitor initor);
	void Shutdown();

	void BegineFrame();
	void Render();
	void EndFrame();

	void OnEvent(sl::Event &e);
	bool OnWindowClose(sl::Event &e);

	bool m_isRunning = true;
	sl::Window *m_pWindow = nullptr;
	sl::LayerStack *m_pLayerStack = nullptr;
};

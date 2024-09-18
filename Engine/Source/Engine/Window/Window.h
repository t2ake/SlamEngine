#pragma once

#include "Event/Event.h"

#include <memory>

namespace sl
{

class RenderContext;

class Window final
{
public:
	static Window &GetInstance()
	{
		static Window s_instance;
		return s_instance;
	}

public:
	Window();
	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;
	Window(Window &&) = delete;
	Window &operator=(Window &&) = delete;
	~Window();

	void Init(std::string_view title, uint32_t width, uint32_t height);
	void Terminate();

	void BegineFrame();
	void EndFrame();

	void *GetNativeWindow() const { return m_pNativeWindow; }
	void SetVSync(bool VSync);
	void CursorModeDisabled();
	void CursorModeNormal();
	void SetCursorPos(float x, float y);
	void SetGlobalCursorPos(float x, float y);

	void SetEventCallback(auto fun) { m_eventCallback = fun; }
	void DespatchEvent(Event &event) { m_eventCallback(event); }

private:
	void *m_pNativeWindow = nullptr;
	std::unique_ptr<RenderContext> m_pRenderContext;
	EventCallback m_eventCallback;
};

} // namespace sl

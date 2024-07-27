#pragma once

#include "Event/Event.h"

#include <memory>

namespace sl
{

class RenderContext;

class Window final
{
public:
	Window() = delete;
	Window(std::string_view title, uint32_t width, uint32_t height);
	~Window();

	void BegineFrame();
	void EndFrame();

	void *GetNativeWindow() const { return m_pNativeWindow; }
	void SetVSync(bool VSync);
	void CursorModeDisabled();
	void CursorModeNormal();
	void SetCursorPos(float x, float y);
	void SetGlobalCursorPos(float x, float y);

	void SetEventCallback(EventCallback fun) { m_eventCallback = std::move(fun); }
	void DespatchEvent(Event &event) { m_eventCallback(event); }

private:
	// No need to delete this pointer.
	void *m_pNativeWindow = nullptr;
	std::unique_ptr<RenderContext> m_pRenderContext;
	EventCallback m_eventCallback;
};

} // namespace sl

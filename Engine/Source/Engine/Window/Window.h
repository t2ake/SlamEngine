#pragma once

#include "Event/Event.h"
#include "RenderCore/RenderContext.h"

#include <memory>

namespace sl
{

class Window final
{
public:
	Window() = delete;

	Window(std::string_view title, uint32_t width, uint32_t height);
	~Window();

	void BegineFrame();
	void EndFrame();

	void *GetNativeWindow() const { return m_pNativeWindow; }

	void SetWidth(uint32_t width) { m_width = width; }
	uint32_t &GetWidth() { return m_width; }
	uint32_t GetWidth() const { return m_width; }

	void SetHeight(uint32_t height) { m_height = height; }
	uint32_t &GetHeight() { return m_height; }
	uint32_t GetHeight() const { return m_height; }

	uint32_t GetMonitorWidth() const { return m_monitorWidth; }
	uint32_t GetMonitorHeight() const { return m_monitorHeight; }

	void SetVSync(bool VSync);

	void CursorModeDisabled();
	void CursorModeNormal();
	void SetCursorPos(float x, float y);
	void SetGlobalCursorPos(float x, float y);

	void SetEventCallback(EventCallback fun) { m_eventCallback = std::move(fun); }
	void DespatchEvent(Event &event) { m_eventCallback(event); }

private:
	void SetCallbacks();

	void *m_pNativeWindow = nullptr;
	std::unique_ptr<RenderContext> m_pRenderContext;

	std::string m_title;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_monitorWidth;
	uint32_t m_monitorHeight;

	EventCallback m_eventCallback = nullptr;
};

} // namespace sl

#pragma once

#include "Event/Event.h"
#include "RenderCore/RenderContext.h"

#include <string>

struct GLFWwindow;

namespace sl
{

class Window final
{
public:
	static Window &GetInstance()
	{
		static Window instance;
		return instance;
	}

protected:
	Window() = default;

public:
	void Init(std::string title, uint32_t width, uint32_t height);
	void Shutdown();

	void BegineFrame();
	void EndFrame();

	GLFWwindow *GetWindow() const { return m_pWindow; }

	void SetWidth(uint32_t width) { m_width = width; }
	uint32_t &GetWidth() { return m_width; }
	uint32_t GetWidth() const { return m_width; }

	void SetHeight(uint32_t height) { m_height = height; }
	uint32_t &GetHeight() { return m_height; }
	uint32_t GetHeight() const { return m_height; }

	void SetVSync(bool VSync);
	bool &GetIsVSync() { return m_isVSync; }
	bool GetIsVSync() const { return m_isVSync; }

	void DisableCursor() const;
	void EnableCursor() const;

	void SetEventCallback(EventCallback cb) { m_eventCallback = std::move(cb); }
	void DespatchEvent(Event &event) { m_eventCallback(event); }

private:

	void SetCallbacks();

	GLFWwindow *m_pWindow = nullptr;
	RenderContext *m_pRenderContext = nullptr;

	std::string m_title = "Default Title";
	uint32_t m_width = 1280;
	uint32_t m_height = 720;
	bool m_isVSync = true;

	EventCallback m_eventCallback = nullptr;
};

} // namespace sl

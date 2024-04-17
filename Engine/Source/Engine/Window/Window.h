#pragma once

#include "Event/Event.h"
#include "RenderCore/RenderingContext.h"

struct GLFWwindow;

namespace sl
{

class Window final
{

public:
	Window() = delete;
	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;
	Window(Window &&) = delete;
	Window &operator=(Window &&) = delete;

	Window(std::string title, uint32_t width, uint32_t height, bool VSync);
	~Window();

	void BegineFrame();
	void Update();
	void Render();
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

	void SetEventCallback(EventCallback cb) { m_eventCallback = std::move(cb); }
	void DespatchEvent(Event &event) { m_eventCallback(event); }

private:
	void Init();
	void Shutdown();

	void SetCallbacks();

	GLFWwindow *m_pWindow = nullptr;
	// PENDIGN
	RenderingContext *m_pRenderingContext = nullptr;

	std::string m_title;
	uint32_t m_width;
	uint32_t m_height;
	bool m_isVSync;

	EventCallback m_eventCallback;
};

} // namespace sl

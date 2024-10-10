#pragma once

#include "Event/Event.h"

#include <glm/glm.hpp>
#include <memory>

struct SDL_WindowEvent;

namespace sl
{

class RenderContext;

enum class VSyncMode : int
{
	On = 1,
	Off = 0,
	Adaptive = -1,
};

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
	void *GetRenderContext() const;

	void SetVSync(VSyncMode mode) const;
	glm::ivec2 GetSize() const;

	void SetEventCallback(auto fun) { m_eventCallback = fun; }

private:
	void PullEvents();
	void ForwardWindowEvents(SDL_WindowEvent &SDLWindowEvent);

	void *m_pNativeWindow = nullptr;
	std::unique_ptr<RenderContext> m_pRenderContext;
	EventCallback m_eventCallback;
};

} // namespace sl

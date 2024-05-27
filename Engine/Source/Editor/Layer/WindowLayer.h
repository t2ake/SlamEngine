#pragma once

#include "Event/Event.h"
#include "Layer/Layer.h"

namespace sl
{

class Window;

}

class WindowLayer : public sl::Layer
{
public:
	WindowLayer() = default;
	virtual ~WindowLayer() override;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;

	void SetWindow(sl::Window *pWindow) { m_pWindow = pWindow; }
	sl::Window *GetWindow() const { return m_pWindow; }

	void SetEventCallback(sl::EventCallback fun) { m_eventCallback = std::move(fun); }

private:
	sl::Window *m_pWindow = nullptr;

	sl::EventCallback m_eventCallback;
};

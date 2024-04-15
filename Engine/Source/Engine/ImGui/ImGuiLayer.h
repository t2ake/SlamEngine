#pragma once

#include "Layer/Layer.h"

namespace sl
{

class Window;

class ImGuiLayer : public Layer
{
public:
	using Layer::Layer;

	ImGuiLayer(Window *pWindow);
	virtual ~ImGuiLayer() override;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnEvent(Event &event) override;

private:
	Window *m_pWindow = nullptr;
};

} // namespace sl

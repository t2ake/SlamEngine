#pragma once

#include "Layer/Layer.h"

namespace sl
{

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	virtual ~ImGuiLayer() override;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;
};

} // namespace sl

#pragma once

#include "Layer/Layer.h"

class RendererLayer : public sl::Layer
{
public:
	RendererLayer() = default;
	virtual ~RendererLayer() override = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;
};

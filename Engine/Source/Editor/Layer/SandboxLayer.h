#pragma once

#include "LayerStack/Layer.h"

class SandboxLayer : public sl::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer() override;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;
};

#pragma once

#include "Layer/Layer.h"

// Basically, SceneLayer is just a OnUpdate / OnEvent controller of ECSWorld.
class SceneLayer : public sl::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;

private:

};

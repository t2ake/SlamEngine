#pragma once

#include "Layer/Layer.h"

#include <glm/vec2.hpp>

class ImGuiLayer : public sl::Layer
{
public:
	ImGuiLayer();
	virtual ~ImGuiLayer() override;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;

private:
	void ShowMenuBar();
	void ShowSceneViewpotr();

	int m_dockSpaceFlag = 0;
	glm::vec2 m_SceneViewportSize{ 0.0f };
};

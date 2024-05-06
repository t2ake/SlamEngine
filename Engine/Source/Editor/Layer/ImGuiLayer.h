#pragma once

#include "Event/Event.h"
#include "Layer/Layer.h"
#include "Scene/ECSWorld.h"

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

	uint32_t GetSceneViewportSizeX() const { return m_viewportSizeX; }
	uint32_t GetSceneViewportSizeY() const { return m_viewportSizeY; }

	void SetEventCallback(sl::EventCallback fun) { m_eventCallback = std::move(fun); }

private:
	void ShowDebugPanels();
	void ShowMenuBar();
	void ShowEntityList();
	void ShowLog();
	void ShowInfo(float deltaTime);
	void ShowDetails();
	void ShowSceneViewport();

	void StartWithText(std::string text);

	// Dock space
	// ImGuiDockNodeFlags m_dockSpaceFlag = ImGuiDockNodeFlags_None
	int m_dockSpaceFlag = 0;
	
	// Menubar
	bool m_debugImGuiDemo = false;
	bool m_debugItemPicker = false;
	bool m_debugIDStack = false;

	// Entity list
	sl::Entity m_selectedEntity;

	// Scene viewport
	uint32_t m_viewportSizeX = 0;
	uint32_t m_viewportSizeY = 0;
	bool m_isSceneViewportFocused = false;
	
	sl::EventCallback m_eventCallback;
};

#pragma once

#include "Event/Event.h"
#include "LayerStack/Layer.h"
#include "Scene/ECSWorld.h"

#include <glm/vec2.hpp>

#include <filesystem>

namespace sl
{

class KeyPressEvent;
class KeyReleaseEvent;
class MouseButtonPressEvent;
class MouseButtonReleaseEvent;
class MouseMoveEvent;

}

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

	void SetEventCallback(auto fun) { m_eventCallback = fun; }

private:
	void ShowDebugPanel();
	void ShowToolOverlay();

	void ShowMenuBar();
	void ShowInfo(float deltaTime);
	void ShowLog();
	void ShowEntityList();
	void ShowAssetBrowser();

	template<class T>
	void DrawComponent(const char *label, auto drawParameters);
	template<class T>
	void AddComponentMenuItem(const char *label);
	void StartWithText(std::string_view text);
	void ShowDetails();

	void ShowImGuizmoOrientation();
	void ShowImGuizmoTransform();
	void ShowSceneViewport();
	void MousePick();

	bool OnKeyPressed(sl::KeyPressEvent& event);
	bool OnKeyRelease(sl::KeyReleaseEvent &event);
	bool OnMouseButtonPress(sl::MouseButtonPressEvent &event);
	bool OnMouseButtonRelease(sl::MouseButtonReleaseEvent &event);
	bool OnMouseMove(sl::MouseMoveEvent &event);

	// Dock space
	int m_dockSpaceFlag = 0; // ImGuiDockNodeFlags m_dockSpaceFlag = ImGuiDockNodeFlags_None
	
	// Debug panel
	bool m_debugImGuiDemo = false;
	bool m_debugStyleEditor = false;
	bool m_debugItemPicker = false;
	bool m_debugIDStack = false;

	// Entity list and many other panels
	sl::Entity m_selectedEntity;

	// Asset browser
	std::filesystem::path m_assetBrowserCrtPath;
	std::string m_fileIconName;
	std::string m_folderIconName;

	// Details
	float m_maxTextSize = 0.0f;

	// Scene viewport
	uint32_t m_sceneViewportWindowPosX = 0;
	uint32_t m_sceneViewportWindowPosY = 0;
	uint32_t m_sceneViewportSizeX = 0;
	uint32_t m_sceneViewportSizeY = 0;
	bool m_isMouseFreeInSceneView = true;

	// ImGuizmo
	int m_imguizmoMode = 7; // ImGuizmo::OPERATION m_imguizmoMode = ImGuizmo::OPERATION::TRANSLATE
	
	sl::EventCallback m_eventCallback;
};

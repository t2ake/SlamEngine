#include "WindowLayer.h"

#include "Event/MouseEvent.h"
#include "Scene/ECSWorld.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include <imgui.h>
#include <imguizmo/ImGuizmo.h>

WindowLayer::~WindowLayer()
{
	delete m_pWindow;
}

void WindowLayer::OnAttach()
{

}

void WindowLayer::OnDetach()
{

}

void WindowLayer::OnEvent(sl::Event &event)
{

}

void WindowLayer::BeginFrame()
{

}

void WindowLayer::OnUpdate(float deltaTime)
{
	auto &camera = sl::ECSWorld::GetEditorCameraComponent();

	if ((!camera.IsUsing() && !ImGui::IsMouseDragging(ImGuiMouseButton_Left)) || ImGuizmo::IsUsing())
	{
		return;
	}

	float newPosX = -1.0f;
	float newPosY = -1.0f;
	const float monitorWidth = (float)m_pWindow->GetMonitorWidth();
	const float monitorHeight = (float)m_pWindow->GetMonitorHeight();
	const glm::vec2 globalMousePos = sl::Input::GetGlobalMousePos();

	if (globalMousePos.x <= 0)
	{
		newPosX = monitorWidth - 2.0f;
		newPosY = globalMousePos.y;
	}
	if (globalMousePos.x >= monitorWidth - 1.0f)
	{
		newPosX = 1.0f;
		newPosY = globalMousePos.y;
	}
	if (globalMousePos.y <= 0)
	{
		newPosX = globalMousePos.x;
		newPosY = monitorHeight - 2.0f;
	}
	if (globalMousePos.y >= monitorHeight - 1.0f)
	{
		newPosX = globalMousePos.x;
		newPosY = 1.0f;
	}

	if (newPosX > 0.0f && newPosY > 0.0f)
	{
		m_pWindow->SetGlobalCursorPos(newPosX, newPosY);

		// To avoid a huge delta mouse position for system which cache the real mouse position.
		ImGui::GetIO().AddMousePosEvent(-FLT_MAX, -FLT_MAX);
		camera.m_mouseLastPos = sl::Input::GetMousePos();
	}
}

void WindowLayer::OnRender()
{

}

void WindowLayer::EndFrame()
{
	m_pWindow->EndFrame();
}

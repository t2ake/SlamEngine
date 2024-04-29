#include "ImGuiLayer.h"

#include "Core/Log.h"
#include "Event/MouseEvent.h"
#include "Event/SceneViewportEvent.h"
#include "RenderCore/RenderCore.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include <imgui/imgui.h>

ImGuiLayer::ImGuiLayer()
{
	SetName("ImGui Layer");
}

ImGuiLayer::~ImGuiLayer()
{

}

void ImGuiLayer::OnAttach()
{

}

void ImGuiLayer::OnDetach()
{

}

void ImGuiLayer::OnEvent(sl::Event &event)
{

}

void ImGuiLayer::BeginFrame()
{

}

void ImGuiLayer::OnUpdate(float deltaTime)
{

}

void ImGuiLayer::OnRender()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), (ImGuiDockNodeFlags)m_dockSpaceFlag);
	ShowMenuBar();
	ShowInfoViewport();
	ShowSceneViewport();
}

void ImGuiLayer::EndFrame()
{

}

void ImGuiLayer::ShowMenuBar()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Options"))
	{
		if (ImGui::MenuItem("NoUndocking", "", (m_dockSpaceFlag & ImGuiDockNodeFlags_NoUndocking) != 0))
		{
			m_dockSpaceFlag ^= ImGuiDockNodeFlags_NoUndocking;
		}

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void ImGuiLayer::ShowInfoViewport()
{
	ImGui::Begin("Info");

	ImGui::Text(m_isSceneViewportFocused ? "true" : "false");

	ImGui::End();
}

void ImGuiLayer::ShowSceneViewport()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene");

	// Scene viewport event stuff
	{
		if (ImGui::IsWindowHovered() && sl::Input::IsMouseButtonPressed(SL_MOUSE_BUTTON_2))
		{
			ImGui::SetWindowFocus();
		}
		if (bool crtFocus = ImGui::IsWindowFocused(); crtFocus != m_isSceneViewportFocused)
		{
			if (crtFocus)
			{
				sl::SceneViewportFocusEvent event;
				m_eventCallback(event);
			}
			else
			{
				sl::SceneViewportLostFocusEvent event;
				m_eventCallback(event);
			}
			m_isSceneViewportFocused = crtFocus;
		}

		auto crtSize = ImGui::GetContentRegionAvail();
		uint32_t crtSizeX = (uint32_t)crtSize.x;
		uint32_t crtSizeY = (uint32_t)crtSize.y;
		if (crtSizeX != m_viewportSizeX || crtSizeY != m_viewportSizeY)
		{
			sl::SceneViewportResizeEvent event{ crtSizeX , crtSizeY };
			m_eventCallback(event);

			m_viewportSizeX = crtSizeX;
			m_viewportSizeY = crtSizeY;
		}
	}

	uint32_t handle = sl::RenderCore::GetMainFrameBuffer()->GetColorAttachmentHandle();
#pragma warning(suppress : 4312)
	ImGui::Image((void *)handle, ImVec2{ (float)m_viewportSizeX, (float)m_viewportSizeY }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	ImGui::End();
	ImGui::PopStyleVar();
}

// For ImGuiLayer::m_dockSpaceFlag
static_assert(std::is_same_v<ImGuiDockNodeFlags, int>);
static_assert(ImGuiDockNodeFlags_None == 0);

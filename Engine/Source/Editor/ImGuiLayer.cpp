#include "ImGuiLayer.h"

#include "Core/Log.h"
#include "Event/Event.h"
#include "RenderCore/RenderCore.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include <imgui/imgui.h>

ImGuiLayer::ImGuiLayer()
{
	SetName("ImGuiLayer");
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
	ShowSceneViewpotr();
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

void ImGuiLayer::ShowSceneViewpotr()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene");

	auto crtSize = ImGui::GetContentRegionAvail();
	if (crtSize.x != m_SceneViewportSize.x || crtSize.y != m_SceneViewportSize.y)
	{
		if (!sl::Input::IsMouseButtonPressed(SL_MOUSE_BUTTON_1))
		{
			m_SceneViewportSize = { crtSize.x, crtSize.y };
			sl::RenderCore::GetMainFrameBuffer()->Resize((uint32_t)m_SceneViewportSize.x, (uint32_t)m_SceneViewportSize.y);
		}
	}

	uint32_t handle = sl::RenderCore::GetMainFrameBuffer()->GetColorAttachmentHandle();
#pragma warning(suppress : 4312)
	ImGui::Image((void *)handle, ImVec2{ m_SceneViewportSize.x, m_SceneViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	ImGui::End();
	ImGui::PopStyleVar();
}

// For ImGuiLayer::m_dockSpaceFlag
static_assert(sizeof(int) == sizeof(ImGuiDockNodeFlags));
static_assert(0 == ImGuiDockNodeFlags_None);

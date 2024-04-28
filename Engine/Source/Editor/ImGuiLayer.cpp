#include "ImGuiLayer.h"

#include "Core/Log.h"
#include "Event/Event.h"
#include "RenderCore/RenderCore.h"
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
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockspace_flags);

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Options"))
	{
		if (ImGui::MenuItem("NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))
		{
			dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking;
		}

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	ImGui::Begin("Scene");
	uint32_t handle = sl::RenderCore::GetFrameBuffer()->GetColorAttachmentHandle();
#pragma warning(suppress : 4312)
	ImGui::Image((void *)handle, ImVec2{ 1280, 720 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();
}

void ImGuiLayer::EndFrame()
{

}

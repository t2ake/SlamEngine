#include "ImGuiLayer.h"

#include "Core/Log.h"
#include "Event/MouseEvent.h"
#include "Event/SceneViewportEvent.h"
#include "ImGui/ImGuiContext.h"
#include "RenderCore/RenderCore.h"
#include "RenderCore/RenderCore.h"
#include "Scene/ECSWorld.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include <imgui/imgui.h>
#include <nameof/nameof.hpp>

#include <format>

ImGuiLayer::ImGuiLayer()
{
	SetName("ImGui Layer");

	m_dockSpaceFlag |= ImGuiDockNodeFlags_NoUndocking;
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
	sl::ImGuiContext::NewFrame();
}

void ImGuiLayer::OnUpdate(float deltaTime)
{
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), (ImGuiDockNodeFlags)m_dockSpaceFlag);

	ShowMenuBar();
	ShowEntityList();
	ShowLog();
	ShowInfo(deltaTime);
	ShowDetails();

	ShowSceneViewport();

	//ImGui::PopStyleVar();
}

void ImGuiLayer::OnRender()
{
	sl::ImGuiContext::Submit();
}

void ImGuiLayer::EndFrame()
{

}

void ImGuiLayer::ShowMenuBar()
{
	static bool ShowImGuiDemo = false;
	static bool ItemPicker = false;

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Settings"))
	{
		if (ImGui::MenuItem("No Undocking", "", m_dockSpaceFlag & ImGuiDockNodeFlags_NoUndocking))
		{
			m_dockSpaceFlag ^= ImGuiDockNodeFlags_NoUndocking;
		}
		if (ImGui::MenuItem("No Resize", "", m_dockSpaceFlag & ImGuiDockNodeFlags_NoResize))
		{
			m_dockSpaceFlag ^= ImGuiDockNodeFlags_NoResize;
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Debug"))
	{
		if (ImGui::MenuItem("Show ImGui Demo", "", ShowImGuiDemo))
		{
			ShowImGuiDemo = !ShowImGuiDemo;
		}
		if (ImGui::MenuItem("Item Picker", "", ItemPicker))
		{
			ItemPicker = !ItemPicker;
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	if (ShowImGuiDemo)
	{
		ImGui::ShowDemoWindow();
	}
	if (ItemPicker)
	{
		ImGui::DebugStartItemPicker();
	}
}

void ImGuiLayer::ShowEntityList()
{
	ImGui::Begin("Entity List");

	auto view = sl::ECSWorld::GetRegistry().view<sl::TagComponent>();
	for (auto entity : view)
	{
		auto &tag = view.get<sl::TagComponent>(entity);

		ImGuiTreeNodeFlags flags = ((m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
		// TODO: hierarchy
		flags |= ImGuiTreeNodeFlags_Leaf;

		bool opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags, tag.m_name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectedEntity = entity;
		}
		if (opened)
		{
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void ImGuiLayer::ShowLog()
{
	ImGui::Begin("Log");



	ImGui::End();
}

void ImGuiLayer::ShowInfo(float deltaTime)
{
	ImGui::Begin("Info");

	ImGui::Text("Backend: %s", nameof::nameof_enum(sl::RenderCore::GetBackend()).data());
	ImGui::Text(std::format("FPS: {}", 1000.0f / deltaTime).c_str());

	ImGui::End();
}

void ImGuiLayer::ShowDetails()
{
	ImGui::Begin("Details");



	ImGui::End();
}

void ImGuiLayer::ShowSceneViewport()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	bool open = true;
	ImGui::Begin("Scene", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

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
				sl::SceneViewportGetFocusEvent event;
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

		sl::RenderCore::GetMainFrameBuffer()->Resize(m_viewportSizeX, m_viewportSizeY);
	}

	uint32_t handle = sl::RenderCore::GetMainFrameBuffer()->GetColorAttachmentHandle();
	ImGui::Image((void *)(uint64_t)handle, ImVec2{ (float)m_viewportSizeX, (float)m_viewportSizeY }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	ImGui::End();
	ImGui::PopStyleVar();
}

// For ImGuiLayer::m_dockSpaceFlag
static_assert(std::is_same_v<ImGuiDockNodeFlags, int>);
static_assert(ImGuiDockNodeFlags_None == 0);

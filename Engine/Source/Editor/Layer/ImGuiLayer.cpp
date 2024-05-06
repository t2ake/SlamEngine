#include "ImGuiLayer.h"

#include "Core/Log.h"
#include "Event/SceneViewportEvent.h"
#include "ImGui/ImGuiContext.h"
#include "RenderCore/RenderCore.h"
#include "Scene/ECSWorld.h"
#include "Window/Input.h"

#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <nameof/nameof.hpp>

namespace
{

SL_FORCEINLINE static glm::vec3 ModVec3(const glm::vec3 &v, float m)
{
	// Why the second argument to glm::modf must accept a left-valued reference?

	return glm::vec3{ std::fmod(v.x, m), std::fmod(v.y, m) , std::fmod(v.z, m) };
}

}

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
	ShowDebugPanels();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), (ImGuiDockNodeFlags)m_dockSpaceFlag);

	ShowMenuBar();
	ShowEntityList();
	ShowLog();
	ShowInfo(deltaTime);
	ShowDetails();
	ShowSceneViewport();
}

void ImGuiLayer::OnRender()
{
	sl::ImGuiContext::Submit();
}

void ImGuiLayer::EndFrame()
{

}

void ImGuiLayer::ShowDebugPanels()
{
	if (m_debugImGuiDemo)
	{
		ImGui::ShowDemoWindow(&m_debugImGuiDemo);
	}
	if (m_debugIDStack)
	{
		ImGui::ShowIDStackToolWindow(&m_debugIDStack);
	}
	if (m_debugItemPicker)
	{
		ImGui::DebugStartItemPicker();
	}
}

void ImGuiLayer::ShowMenuBar()
{
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
		ImGui::MenuItem("Show ImGui Demo", "", &m_debugImGuiDemo);
		ImGui::MenuItem("Item Picker", "", &m_debugItemPicker);
		ImGui::MenuItem("ID Stack", "", &m_debugIDStack);
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void ImGuiLayer::ShowEntityList()
{
	ImGui::Begin("Entity List");

	auto view = sl::ECSWorld::GetRegistry().view<sl::TagComponent>();
	for (auto entity : view)
	{
		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_FramePadding |
			ImGuiTreeNodeFlags_SpanFullWidth |
			ImGuiTreeNodeFlags_Leaf; // TODO: hierarchy
		if (m_selectedEntity == entity)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		auto &tag = view.get<sl::TagComponent>(entity);
		if (ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags, tag.m_name.c_str()))
		{
			if (ImGui::IsItemClicked())
			{
				m_selectedEntity = entity;
			}
			ImGui::TreePop();
		}
	}

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		m_selectedEntity = sl::Entity{};
	}

	ImGui::End();
}

void ImGuiLayer::ShowLog()
{
	ImGui::Begin("Log");

	ImGui::Text("TODO");

	ImGui::End();
}

void ImGuiLayer::ShowInfo(float deltaTime)
{
	ImGui::Begin("Info");

	ImGui::Text("Backend: %s", nameof::nameof_enum(sl::RenderCore::GetBackend()).data());
	ImGui::Text("%.2f ms/frame (%.2f fps)", deltaTime, 1000.0f / deltaTime);

	ImGui::End();
}

void ImGuiLayer::ShowDetails()
{
	ImGui::Begin("Details");

	if (!m_selectedEntity)
	{
		ImGui::End();
		return;
	}

	constexpr ImGuiTreeNodeFlags DefaultFlags =
		ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_CollapsingHeader;

	if (auto *pTag = m_selectedEntity.TryGetComponent<sl::TagComponent>(); pTag)
	{
		if (ImGui::TreeNodeEx("##Tag", DefaultFlags, "Tag"))
		{
			std::string &name = pTag->m_name;

			constexpr size_t BufferSize = 32;
			SL_EDITOR_ASSERT(BufferSize >= name.size());

			char buffer[BufferSize] = { 0 };
			memcpy(buffer, name.c_str(), name.size());

			ImGui::Text("Name    ");
			ImGui::SameLine();
			if (ImGui::InputText("##Name", buffer, BufferSize))
			{
				name = std::string{ buffer };
				if (name.empty())
				{
					name = "Default Name";
				}
			}
		}
		ImGui::Separator();
	}

	if (auto *pTransform = m_selectedEntity.TryGetComponent<sl::TransformComponent>(); pTransform)
	{
		if (ImGui::TreeNodeEx("##Transform", DefaultFlags, "Transform"))
		{
			glm::vec3 &position = pTransform->m_position;
			glm::vec3 &rotation = pTransform->m_rotation;
			glm::vec3 &scale = pTransform->m_scale;

			bool cameraMayBeDirty = false;

			ImGui::Text("Position");
			ImGui::SameLine();
			if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.1f))
			{
				cameraMayBeDirty = true;
			}

			ImGui::Text("Rotation");
			ImGui::SameLine();
			glm::vec3 ratationDegree = ModVec3(glm::degrees(rotation), 360.0f);
			if (ImGui::DragFloat3("##Rotation", glm::value_ptr(ratationDegree), 0.1f))
			{
				rotation = glm::radians(std::move(ratationDegree));
				cameraMayBeDirty = true;
			}

			ImGui::Text("Scale   ");
			ImGui::SameLine();
			ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f);
			
			if (auto *pCamera = m_selectedEntity.TryGetComponent<sl::CameraComponent>(); pCamera && cameraMayBeDirty)
			{
				pCamera->m_isDirty = true;
			}
		}
		ImGui::Separator();
	}

	if (auto *pCamera = m_selectedEntity.TryGetComponent<sl::CameraComponent>(); pCamera)
	{
		if (ImGui::TreeNodeEx("##Camera", DefaultFlags, "Camera"))
		{
			constexpr const char *ProjectionTypeNames[] =
			{
				nameof::nameof_enum(sl::ProjectionType::Perspective).data(),
				nameof::nameof_enum(sl::ProjectionType::Orthographic).data(),
			};

			ImGui::Text("Projection Type");
			ImGui::SameLine();
			const char *crtProjectionTypeName = ProjectionTypeNames[(size_t)pCamera->m_projectionType];
			if (ImGui::BeginCombo("##Projection Type", crtProjectionTypeName, ImGuiComboFlags_WidthFitPreview))
			{
				for (size_t i = 0; i < 2; ++i)
				{
					bool isSelected = (ProjectionTypeNames[i] == crtProjectionTypeName);
					if (ImGui::Selectable(ProjectionTypeNames[i], isSelected))
					{
						pCamera->m_projectionType = (sl::ProjectionType)i;
						pCamera->m_isDirty = true;
					}
				}
				ImGui::EndCombo();
			}

			if (sl::ProjectionType::Perspective == pCamera->m_projectionType)
			{
				ImGui::Text("FOV ");
				ImGui::SameLine();
				float fovDegrees = glm::degrees(pCamera->m_fov);
				if (ImGui::DragFloat("##FOV", &fovDegrees, 0.1f))
				{
					pCamera->m_fov = glm::radians(fovDegrees);
					pCamera->m_isDirty = true;
				}

				ImGui::Text("Near");
				ImGui::SameLine();
				if (ImGui::DragFloat("##Near", &(pCamera->m_nearPlane), 0.1f))
				{
					pCamera->m_isDirty = true;
				}

				ImGui::Text("Far ");
				ImGui::SameLine();
				if (ImGui::DragFloat("##Far", &(pCamera->m_farPlane), 0.1f))
				{
					pCamera->m_isDirty = true;
				}
			}
			else if (sl::ProjectionType::Orthographic == pCamera->m_projectionType)
			{
				ImGui::Text("Size");
				ImGui::SameLine();
				if (ImGui::DragFloat("##Size", &(pCamera->m_orthoSize), 0.1f))
				{
					pCamera->m_isDirty = true;
				}

				ImGui::Text("Near");
				ImGui::SameLine();
				if (ImGui::DragFloat("##Near", &(pCamera->m_orthoNearClip), 0.1f))
				{
					pCamera->m_isDirty = true;
				}

				ImGui::Text("Far ");
				ImGui::SameLine();
				if (ImGui::DragFloat("##Far", &(pCamera->m_orthoFarClip), 0.1f))
				{
					pCamera->m_isDirty = true;
				}
			}
		}
		ImGui::Separator();
	}

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

			sl::RenderCore::GetMainFrameBuffer()->Resize(m_viewportSizeX, m_viewportSizeY);
		}
	}

	uint32_t handle = sl::RenderCore::GetMainFrameBuffer()->GetColorAttachmentHandle();
	ImGui::Image((void *)(uint64_t)handle, ImVec2{ (float)m_viewportSizeX, (float)m_viewportSizeY }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	ImGui::End();
	ImGui::PopStyleVar();
}

// For ImGuiLayer::m_dockSpaceFlag
static_assert(std::is_same_v<ImGuiDockNodeFlags, int>);
static_assert(ImGuiDockNodeFlags_None == 0);

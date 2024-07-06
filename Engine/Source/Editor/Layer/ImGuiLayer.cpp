#include "ImGuiLayer.h"

#include "Core//Path.hpp"
#include "Core/EnumOf.hpp"
#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/SceneViewportEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "RenderCore/RenderCore.h"
#include "Resource/Font.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneSerializer.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>
#include <implot/implot.h>
#include <nameof/nameof.hpp>

namespace
{

constexpr ImGuiWindowFlags OverlayButtonFlags =
	ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoBackground;

constexpr ImGuiTreeNodeFlags DefaultSubTreeFlags =
	ImGuiTreeNodeFlags_NoTreePushOnOpen |
	ImGuiTreeNodeFlags_NoAutoOpenOnLog |
	ImGuiTreeNodeFlags_DefaultOpen |
	ImGuiTreeNodeFlags_OpenOnDoubleClick |
	ImGuiTreeNodeFlags_OpenOnArrow |
	ImGuiTreeNodeFlags_SpanAvailWidth;

constexpr ImGuiTreeNodeFlags DefaultTreeFlags =
	DefaultSubTreeFlags |
	ImGuiTreeNodeFlags_Framed |
	ImGuiTreeNodeFlags_AllowOverlap;

// From ImPlot
struct ScrollingBuffer
{
	ScrollingBuffer(int max_size = 1024)
	{
		MaxSize = max_size;
		Offset = 0;
		Data.reserve(MaxSize);
	}
	void AddPoint(float x, float y)
	{
		if (Data.size() < MaxSize)
		{
			Data.push_back(ImVec2(x, y));
		}
		else
		{
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}
	}
	void Erase()
	{
		if (Data.size() > 0)
		{
			Data.shrink(0);
			Offset = 0;
		}
	}

	int MaxSize;
	int Offset;
	ImVector<ImVec2> Data;
};

SL_FORCEINLINE float GetTitleBarSize()
{
	static float s_size = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
	return s_size;
}

SL_FORCEINLINE void RightClickFocus()
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		ImGui::SetWindowFocus();
	}
}

SL_FORCEINLINE bool AlignButton(const char *label, float align = 0.5f, float customOffset = 0.0f)
{
	float size = ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;
	float offset = (avail - size) * align;

	if (offset > 0.0f)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset + customOffset);
	}

	return ImGui::Button(label);
}

glm::vec3 RotationModAndRepeat(const glm::vec3 &v)
{
	glm::vec3 ret = v;
	for (glm::vec3::length_type i = 0; i < 3; ++i)
	{
		while (ret[i] < 180.0f)
		{
			ret[i] += 360.0f;
		}
		while (ret[i] > 180.0f)
		{
			ret[i] -= 360.0f;
		}
	}

	return ret;
}

} // namespace

ImGuiLayer::ImGuiLayer()
{
	m_dockSpaceFlag |= ImGuiDockNodeFlags_NoUndocking;
	m_selectedEntity = sl::ECSWorld::GetEditorCameraEntity();
	m_assetBrowserCrtPath = sl::Path::AssetPath;
	
	auto pFileIconTextureResource = std::make_unique<sl::TextureResource>(
		sl::Path::FromeAsset("Texture/FileIcon.png"), SL_SAMPLER_REPEAT | SL_SAMPLER_TRILINEAR);
	sl::ResourceManager::AddTextureResource("FileIcon", std::move(pFileIconTextureResource));
	m_fileIconName = "FileIcon";

	auto pFolderIconTextureResource = std::make_unique<sl::TextureResource>(
		sl::Path::FromeAsset("Texture/FolderIcon.png"), SL_SAMPLER_REPEAT | SL_SAMPLER_TRILINEAR);
	sl::ResourceManager::AddTextureResource("FolderIcon", std::move(pFolderIconTextureResource));
	m_folderIconName = "FolderIcon";
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
	sl::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<sl::KeyPressEvent>(BIND_EVENT_CALLBACK(ImGuiLayer::OnKeyPressed));
}

void ImGuiLayer::BeginFrame()
{

}

void ImGuiLayer::OnUpdate(float deltaTime)
{
	sl::ImGuiContext::NewFrame();

	ShowDebugPanel();
	ShowToolOverlay();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), (ImGuiDockNodeFlags)m_dockSpaceFlag);

	ShowMenuBar();
	ShowInfo(deltaTime);
	ShowLog();
	ShowEntityList();
	ShowAssetBrowser();
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

void ImGuiLayer::ShowDebugPanel()
{
	if (m_debugImGuiDemo)
	{
		ImGui::ShowDemoWindow(&m_debugImGuiDemo);
	}
	if (m_debugStyleEditor)
	{
		ImGui::ShowStyleEditor();
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

void ImGuiLayer::ShowToolOverlay()
{
	constexpr float ToolOverlayOffset = 10.0f;
	ImGui::SetNextWindowPos(ImVec2{
		(float)m_sceneViewportWindowPosX + ToolOverlayOffset,
		(float)m_sceneViewportWindowPosY + ToolOverlayOffset + GetTitleBarSize()});

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
	ImGui::Begin("Tools", nullptr, OverlayButtonFlags);
	ImGui::PopStyleVar();

	constexpr std::array<int, 4> Operations =
	{
		-1,
		ImGuizmo::OPERATION::TRANSLATE,
		ImGuizmo::OPERATION::ROTATE,
		ImGuizmo::OPERATION::SCALE,
	};
	constexpr std::array<const char *, 4> Icons =
	{
		// TODO: Icon
		"M", "T", "R", "S",
	};
	auto SelectableButton = [this](size_t index)
	{
		const int op = Operations[index];
		bool selected = false;

		if (m_imguizmoMode == op)
		{
			selected = true;
		}

		if (selected)
		{
			ImGui::PopStyleVar();
		}
		if (ImGui::Button(Icons[index], ImVec2{ 32.0f, 32.0f }))
		{
			m_imguizmoMode = op;
		}
		if (selected)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
		}
	};

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
	for (size_t i = 0; i < 4; ++i)
	{
		SelectableButton(i);
	}
	ImGui::PopStyleVar();

	ImGui::End();
}

void ImGuiLayer::ShowMenuBar()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{
			SL_LOG_ERROR("TODO: New scene button");
		}
		if (ImGui::MenuItem("Open"))
		{
			sl::SceneSerializer::DeserializeYAML("DemoScene");
		}
		if (ImGui::MenuItem("Save"))
		{
			sl::SceneSerializer::SerializeYAML("DemoScene");
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Exit"))
		{
			sl::WindowCloseEvent event;
			m_eventCallback(event);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Setting"))
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
		ImGui::MenuItem("ImGui Demo", "", &m_debugImGuiDemo);
		ImGui::MenuItem("Style Editor", "", &m_debugStyleEditor);
		ImGui::MenuItem("ID Stack", "", &m_debugIDStack);
		ImGui::MenuItem("Item Picker", "", &m_debugItemPicker);
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void ImGuiLayer::ShowInfo(float deltaTime)
{
	ImGui::Begin("Info");
	RightClickFocus();

	// Infos
	ImGui::Text("Backend: %s", nameof::nameof_enum(sl::RenderCore::GetBackend()).data());
	ImGui::Separator();

	// FPS plot
	static float s_sumTime = 0.0f; // Stores in millisecond.
	static float s_deltaTimeMultiplier = 10.0f;
	static ScrollingBuffer s_coastBuffer;
	static ScrollingBuffer s_fpsBuffer;

	constexpr float History = 1000.0f;
	constexpr float Delay = 1000.0f;

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Delta Time Multiplier:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::DragFloat("##DeltaTimeMultiplier", &s_deltaTimeMultiplier, 1.0f, 1.0f, 1000.0f);

	s_sumTime += deltaTime;
	if (s_sumTime > Delay)
	{
		// Waiting for programme to stabilise.
		s_coastBuffer.AddPoint(s_sumTime, s_deltaTimeMultiplier * deltaTime);
		s_fpsBuffer.AddPoint(s_sumTime, 1000.0f / deltaTime);
	}

	constexpr ImPlotFlags PlotFlag = ImPlotFlags_CanvasOnly;
	constexpr ImPlotAxisFlags AxisFlagX = ImPlotAxisFlags_NoTickLabels;
	constexpr ImPlotAxisFlags AxisFlagY = ImPlotAxisFlags_AutoFit;
	if (ImPlot::BeginPlot("Monitor", ImVec2(-1.0f, 150.0f), PlotFlag))
	{
		ImPlot::SetupAxes(nullptr, nullptr, AxisFlagX, AxisFlagY);
		ImPlot::SetupAxisLimits(ImAxis_X1, s_sumTime - History, s_sumTime, ImGuiCond_Always);
		ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
		if (s_sumTime > Delay)
		{
			ImPlot::PlotShaded("Coast in ms",
				&s_coastBuffer.Data[0].x,
				&s_coastBuffer.Data[0].y,
				s_coastBuffer.Data.size(),
				0, 0,
				s_coastBuffer.Offset,
				2 * sizeof(float));

			ImPlot::PlotLine("FPS",
				&s_fpsBuffer.Data[0].x,
				&s_fpsBuffer.Data[0].y,
				s_fpsBuffer.Data.size(),
				0,
				s_fpsBuffer.Offset,
				2 * sizeof(float));
		}
		ImPlot::EndPlot();
	}

	ImGui::End();
}

void ImGuiLayer::ShowLog()
{
	ImGui::Begin("Log");
	RightClickFocus();

	ImGui::Text("TODO");

	ImGui::End();
}

void ImGuiLayer::ShowEntityList()
{
	ImGui::Begin("Entity List");
	RightClickFocus();

	if (AlignButton("Add New Entity"))
	{
		sl::ECSWorld::CreateEntity();
	}
	ImGui::Separator();

	auto view = sl::ECSWorld::GetRegistry().view<sl::TagComponent>();
	for (auto entity : view)
	{
		ImGui::PushID((void *)(uint64_t)(uint32_t)entity);

		// TODO: hierarchy
		ImGuiTreeNodeFlags treeNodeFlag =
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_Leaf;
		if (m_selectedEntity == entity)
		{
			treeNodeFlag |= ImGuiTreeNodeFlags_Selected;
		}

		auto &tag = view.get<sl::TagComponent>(entity);
		bool nodeOpen = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, treeNodeFlag, tag.m_name.c_str());

		// Left click to select entity.
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			m_selectedEntity = entity;
		}

		// Right click to open an entity popup.
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("EntityPopupContext");
		}
		if (ImGui::BeginPopup("EntityPopupContext"))
		{
			if (ImGui::MenuItem("Destory Entity"))
			{
				sl::ECSWorld::DestroyEntity(entity);
				if (m_selectedEntity == entity)
				{
					m_selectedEntity.Reset();
				}
			}
			ImGui::EndPopup();
		}

		if (nodeOpen)
		{
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	// Left click at enpty space to clear selected entity.
	if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		m_selectedEntity.Reset();
	}

	ImGui::End();
}

void ImGuiLayer::ShowAssetBrowser()
{
	ImGui::Begin("Asset Browser");

	// Disable the back button if the current path reaches the outermost path.
	std::string crtPath = m_assetBrowserCrtPath.generic_string();
	bool backButtonDisabled = !sl::Path::Contain(sl::Path::AssetPath, crtPath);
	if (backButtonDisabled)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button("<-"))
	{
		m_assetBrowserCrtPath = m_assetBrowserCrtPath.parent_path();
	}
	if (backButtonDisabled)
	{
		ImGui::EndDisabled();
	}

	ImGui::SameLine();
	ImGui::Text(crtPath.c_str());
	ImGui::Separator();

	constexpr ImVec2 ButtonSize{ 100.0f, 100.0f };
	float columnSize = ButtonSize.x + ImGui::GetStyle().ItemSpacing.x;
	uint32_t columnCount = uint32_t(ImGui::GetContentRegionAvail().x / columnSize);
	columnCount = std::max(1U, columnCount);
	ImGui::Columns(columnCount, "Asset Browser Colums", false);

	uint32_t columnIndex = 0;
	for (const auto &it : std::filesystem::directory_iterator(m_assetBrowserCrtPath))
	{
		// WARNING: Will cause an error if we dont have an existing imgui.ini file.
		columnIndex = columnIndex >= columnCount ? 0 : columnIndex;
		ImGui::SetColumnWidth(columnIndex++, columnSize);

		std::string fileName = it.path().filename().generic_string();
		ImGui::PushID(fileName.c_str());

		const bool isDirectory = it.is_directory();
		if (isDirectory)
		{
			if (auto *pTextureResource = sl::ResourceManager::GetTextureResource(m_folderIconName); pTextureResource->IsReady())
			{
				ImGui::ImageButton(fileName.c_str(), (ImTextureID)(uint64_t)pTextureResource->GetTexture()->GetHandle(),
					ButtonSize, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
			}
		}
		else // Is file
		{
			if (auto *pTextureResource = sl::ResourceManager::GetTextureResource(m_fileIconName); pTextureResource->IsReady())
			{
				ImGui::ImageButton(fileName.c_str(), (ImTextureID)(uint64_t)pTextureResource->GetTexture()->GetHandle(),
					ButtonSize, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
			}
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (isDirectory)
			{
				m_assetBrowserCrtPath = it.path();
			}
		}

		ImGui::TextWrapped(fileName.c_str());

		ImGui::PopID();
		ImGui::NextColumn();
	}

	ImGui::Columns(1);
	ImGui::End();
}

template<class T, class Fun>
void ImGuiLayer::DrawComponent(const char *label, Fun uiFunction)
{
	if (T *pComponent = m_selectedEntity.TryGetComponent<T>(); pComponent)
	{
		ImGui::PushID(nameof::nameof_type<T>().data());

		// Draw tree node
		ImGui::PushFont(sl::Font::GetBold());
		bool componentTreeOpen = ImGui::TreeNodeEx(label, DefaultTreeFlags, label);
		ImGui::PopFont();

		// Draw component menu button
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (AlignButton(" : ", 1.0f, ImGui::GetStyle().WindowPadding.x / 2.0f))
		{
			// Don't know why WindowPadding.x / 2 works perfect here.
			ImGui::OpenPopup("ComponentPopup");
		}
		ImGui::PopStyleColor();

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentPopup"))
		{
			if (ImGui::MenuItem("Reset Component"))
			{
				pComponent->Reset();
				if constexpr (std::is_same_v<T, sl::TransformComponent>)
				{
					if (auto pCamera = m_selectedEntity.TryGetComponent<sl::CameraComponent>(); pCamera)
					{
						pCamera->m_isDirty = true;
					}
				}
			}
			if constexpr (!std::is_same_v<T, sl::TagComponent> && !std::is_same_v<T, sl::TransformComponent>)
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
			}
			ImGui::EndPopup();
		}

		// Draw component specific items
		if (componentTreeOpen)
		{
			uiFunction(pComponent);
		}

		if (removeComponent)
		{
			m_selectedEntity.RemoveComponent<T>();
			m_maxTextSize = 56.0f;
		}

		ImGui::Separator();
		ImGui::PopID();
	}
}

void ImGuiLayer::StartWithText(std::string_view text)
{
	static sl::Entity s_crtEntity;
	if (s_crtEntity != m_selectedEntity)
	{
		// ImGui::CalcTextSize("Position").x == 56.0f
		// ImGui::CalcTextSize("Rotation").x == 56.0f
		// Just a little trick to avoid Tag Component flickering when it is rendered the first time,
		// as we known every entity must hold both Tag and Transform component.
		m_maxTextSize = 56.0f;
		s_crtEntity = m_selectedEntity;
	}

	float crtTextSize = ImGui::CalcTextSize(text.data()).x;
	m_maxTextSize = std::max(m_maxTextSize, crtTextSize);

	float offset = ImGui::GetStyle().IndentSpacing + ImGui::GetFontSize();
	ImGui::SetCursorPosX(offset);
	ImGui::AlignTextToFramePadding();
	ImGui::Text(text.data());

	// TODO: Some hard code size here, need to be parameterised in the future.
	ImGui::SameLine(m_maxTextSize + 50.0f);
	ImGui::SetNextItemWidth(-8.0f);
}

template<class T>
void ImGuiLayer::AddComponent(const char *label)
{
	if (ImGui::MenuItem(label))
	{
		if (!m_selectedEntity.TryGetComponent<T>())
		{
			m_selectedEntity.AddComponent<T>();
		}
		else
		{
			SL_LOG_WARN("Entity \"{}\" already has component \"{}\"", m_selectedEntity.GetComponent<sl::TagComponent>().m_name, label);
		}
	}
}

void ImGuiLayer::ShowDetails()
{
	ImGui::Begin("Details");
	RightClickFocus();

	if (!m_selectedEntity)
	{
		ImGui::End();
		return;
	}

	// Draw Tag component
	DrawComponent<sl::TagComponent>("Tag", [this](sl::TagComponent *pComponent)
	{
		std::string &name = pComponent->m_name;
		constexpr size_t BufferSize = 256;
		SL_ASSERT(BufferSize > name.size(),
			"ImGui ensure that InputText() returns a null-terminated character array, "
			"it also means that character buffer[BufferSize - 1] will be discard.");

		char buffer[BufferSize] = { 0 };
		memcpy(buffer, name.c_str(), name.size());

		StartWithText("Name");
		if (ImGui::InputText("##Name", buffer, BufferSize))
		{
			name = std::string{ buffer };
			if (name.empty())
			{
				name = "Default Name";
			}
		}
	});

	// Draw Transform component
	DrawComponent<sl::TransformComponent>("Transform", [this](sl::TransformComponent *pComponent)
	{
		bool cameraMayBeDirty = false;

		glm::vec3 &position = pComponent->m_position;
		StartWithText("Position");
		if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.1f))
		{
			cameraMayBeDirty = true;
		}

		glm::vec3 ratationDegrees = RotationModAndRepeat(pComponent->GetRotationDegrees());
		StartWithText("Rotation");
		if (ImGui::DragFloat3("##Rotation", glm::value_ptr(ratationDegrees), 0.1f))
		{
			pComponent->SetRotationDegrees(ratationDegrees);
			cameraMayBeDirty = true;
		}

		glm::vec3 &scale = pComponent->m_scale;
		StartWithText("Scale");
		ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f);

		if (cameraMayBeDirty)
		{
			if (auto *pCamera = m_selectedEntity.TryGetComponent<sl::CameraComponent>(); pCamera)
			{
				pCamera->m_isDirty = true; 
			}
		}
	});

	// Draw Camera component
	DrawComponent<sl::CameraComponent>("Camera", [this](sl::CameraComponent *pComponent)
	{
		constexpr size_t Count = nameof::enum_count<sl::ProjectionType>();
		constexpr std::array<const char *, Count> ProjectionTypeNames =
		{
			nameof::nameof_enum(sl::ProjectionType::Perspective).data(),
			nameof::nameof_enum(sl::ProjectionType::Orthographic).data(),
		};

		StartWithText("Projection");
		if (ImGui::BeginCombo("##Projection", ProjectionTypeNames[(size_t)pComponent->m_projectionType], ImGuiComboFlags_WidthFitPreview))
		{
			for (size_t i = 0; i < Count; ++i)
			{
				if (ImGui::Selectable(ProjectionTypeNames[i], i == (size_t)pComponent->m_projectionType))
				{
					pComponent->m_projectionType = (sl::ProjectionType)i;
					pComponent->m_isDirty = true;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Separator();

		if (ImGui::TreeNodeEx("##Perspective", DefaultSubTreeFlags, "Perspective"))
		{
			ImGui::Indent();

			float fovDegrees = glm::degrees(pComponent->m_fov);
			StartWithText("FOV");
			if (ImGui::DragFloat("##FOV", &fovDegrees, 0.1f, 1.0f, 120.0f))
			{
				pComponent->m_fov = glm::radians(fovDegrees);
				pComponent->m_isDirty = true;
			}

			StartWithText("Near Plane");
			if (ImGui::DragFloat("##NearPlane", &(pComponent->m_nearPlane), 0.001f, 0.01f, 1.0f))
			{
				pComponent->m_isDirty = true;
			}

			StartWithText("Far Plane");
			if (ImGui::DragFloat("##FarPlane", &(pComponent->m_farPlane), 100.0f, 1.0f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}

			ImGui::Unindent();
		}
		ImGui::Separator();

		if (ImGui::TreeNodeEx("##Orthographic", DefaultSubTreeFlags, "Orthographic"))
		{
			ImGui::Indent();

			StartWithText("Size");
			if (ImGui::DragFloat("##Size", &(pComponent->m_orthoSize), 100.0f, 1.0f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}

			StartWithText("Near Clip");
			if (ImGui::DragFloat("##NearClip", &(pComponent->m_orthoNearClip), 100.0f, -100000.0f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}

			StartWithText("Far Clip");
			if (ImGui::DragFloat("##FarClip", &(pComponent->m_orthoFarClip), 100.0f, -100000.0f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}

			ImGui::Unindent();
		}
		ImGui::Separator();

		if (ImGui::TreeNodeEx("##Controller", DefaultSubTreeFlags, "Controller"))
		{
			ImGui::Indent();

			float rotateSpeedDegrees = glm::degrees(pComponent->m_rotateSpeed);
			StartWithText("Rotate Speed");
			if (ImGui::DragFloat("##RotateSpeed", &rotateSpeedDegrees, 0.001f, 0.001f, 0.5f))
			{
				pComponent->m_rotateSpeed = glm::radians(rotateSpeedDegrees);
			}

			StartWithText("Move Speed");
			ImGui::DragFloat("##MoveSpeed", &(pComponent->m_maxMoveSpeed), 0.001f, 0.001f, 0.5f);

			StartWithText("Acceleration");
			ImGui::DragFloat("##Acceleration", &(pComponent->m_maxSpeedToAcceleration), 0.001f, 0.001f, 0.5f);

			StartWithText("Shift Multiplier");
			ImGui::DragFloat("##ShiftMultiplier", &(pComponent->m_moveSpeedKeyShiftMultiplier), 0.01f, 0.1f, 10.0f);

			StartWithText("Scroll Multiplier");
			ImGui::DragFloat("##ScrollMultiplier", &(pComponent->m_moveSpeedMouseScrollMultiplier), 0.01f, 0.1f, 10.0f);

			ImGui::Unindent();
		}
	});

	// Draw Rendering component
	DrawComponent<sl::RenderingComponent>("Rendering", [this](sl::RenderingComponent *pComponent)
	{
		StartWithText("Base Shader");
		ImGui::Text(pComponent->m_optBaseShaderResourceName ? pComponent->m_optBaseShaderResourceName->c_str() : "");

		StartWithText("ID Shader");
		ImGui::Text(pComponent->m_optIDShaderResourceName ? pComponent->m_optIDShaderResourceName->c_str() : "");

		StartWithText("Texture");
		ImGui::Text(pComponent->m_optTextureResourceName ? pComponent->m_optTextureResourceName->c_str() : "");

		StartWithText("Mesh");
		ImGui::Text(pComponent->m_optMeshResourceName ? pComponent->m_optMeshResourceName->c_str() : "");
	});

	// Draw Cornerstone component
	DrawComponent<sl::CornerstoneComponent>("Cornerstone", [this](sl::CornerstoneComponent *pComponent)
	{
		StartWithText("Info");
		ImGui::TextWrapped(pComponent->m_info.c_str());
	});

	// Add component button
	if (AlignButton("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}
	if (ImGui::BeginPopup("AddComponent"))
	{
		AddComponent<sl::CameraComponent>("Camera");
		AddComponent<sl::RenderingComponent>("Rendering");
		ImGui::EndPopup();
	}

	ImGui::End();
}

void ImGuiLayer::ShowImGuizmoOrientation()
{
	constexpr float Length = 100.0f;
	ImVec2 pos = ImVec2{
		(float)m_sceneViewportWindowPosX + (float)m_sceneViewportSizeX - Length,
		(float)m_sceneViewportWindowPosY + GetTitleBarSize() };

	auto view = sl::ECSWorld::GetEditorCameraComponent().GetView();
	ImGuizmo::ViewManipulate(glm::value_ptr(view), Length, pos, ImVec2{ Length , Length }, 0);
}

void ImGuiLayer::ShowImGuizmoTransform()
{
	if (sl::ECSWorld::GetEditorCameraEntity() == m_selectedEntity)
	{
		return;
	}

	auto &camera = sl::ECSWorld::GetEditorCameraComponent();
	if (camera.IsUsing())
	{
		ImGuizmo::Enable(false);
	}
	else
	{
		ImGuizmo::Enable(true);
	}

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetOrthographic(sl::ProjectionType::Orthographic == camera.m_projectionType ? true : false);
	ImGuizmo::AllowAxisFlip(false);
	ImGuizmo::SetRect((float)m_sceneViewportWindowPosX, (float)m_sceneViewportWindowPosY + GetTitleBarSize(),
		(float)m_sceneViewportSizeX, (float)m_sceneViewportSizeY);

	const glm::mat4 &view = camera.GetView();
	const glm::mat4 &projection = camera.GetProjection();

	auto &transform = m_selectedEntity.GetComponent<sl::TransformComponent>();
	glm::mat4 manipulatedTransform = transform.GetTransform();

	bool useSnap = ImGui::IsKeyDown(ImGuiKey_LeftCtrl);
	float snap = (ImGuizmo::OPERATION::ROTATE == m_imguizmoMode ? 45.0f : 0.5f);
	float snaps[] = { snap, snap, snap };

	ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
		(ImGuizmo::OPERATION)m_imguizmoMode, ImGuizmo::LOCAL, glm::value_ptr(manipulatedTransform),
		nullptr, useSnap ? snaps : nullptr);

	if(ImGuizmo::IsUsing())
	{
		glm::vec3 newPosition, newRotation, newScale;
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(manipulatedTransform),
			glm::value_ptr(newPosition), glm::value_ptr(newRotation), glm::value_ptr(newScale));

		transform.m_position = newPosition;
		transform.m_rotation += glm::radians(newRotation) - transform.m_rotation;
		transform.m_scale = newScale;
	}
}

void ImGuiLayer::MousePick()
{
	// Origin is on the upper left of scene viewport.
	uint32_t mouseLocalPosX = (uint32_t)ImGui::GetMousePos().x - m_sceneViewportWindowPosX;
	uint32_t mouseLocalPosY = (uint32_t)ImGui::GetMousePos().y - (m_sceneViewportWindowPosY + (uint32_t)GetTitleBarSize());
	SL_ASSERT(mouseLocalPosX >= 0 && mouseLocalPosY >= 0 &&
		mouseLocalPosX < m_sceneViewportSizeX && mouseLocalPosY < m_sceneViewportSizeY);

	auto *pEntityIDFB = sl::RenderCore::GetEntityIDFramebuffer();
	int entityID = pEntityIDFB->ReadPixel(0, mouseLocalPosX, mouseLocalPosY);

	if (entityID < 0)
	{
		m_selectedEntity.Reset();
		return;
	}

	sl::Entity crtEntity{ (uint32_t)entityID };
	if (crtEntity == m_selectedEntity)
	{
		return;
	}

	SL_LOG_TRACE("Select enttiy Name: \"{}\", ID: {}, Mouse position: ({}, {})",
		crtEntity.GetComponent<sl::TagComponent>().m_name.c_str(), entityID,
		mouseLocalPosX, mouseLocalPosY);

	m_selectedEntity = crtEntity;
}

void ImGuiLayer::ShowSceneViewport()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PopStyleVar();
	RightClickFocus();

	m_sceneViewportWindowPosX = (uint32_t)ImGui::GetWindowPos().x;
	m_sceneViewportWindowPosY = (uint32_t)ImGui::GetWindowPos().y;

	// Resize event
	uint32_t crtSceneViewportSizeX = (uint32_t)ImGui::GetContentRegionAvail().x;
	uint32_t crtSceneViewportSizeY = (uint32_t)ImGui::GetContentRegionAvail().y;
	if (crtSceneViewportSizeX != m_sceneViewportSizeX || crtSceneViewportSizeY != m_sceneViewportSizeY)
	{
		m_sceneViewportSizeX = crtSceneViewportSizeX;
		m_sceneViewportSizeY = crtSceneViewportSizeY;

		sl::SceneViewportResizeEvent event{ m_sceneViewportSizeX , m_sceneViewportSizeY };
		m_eventCallback(event);
	}

	// Draw main frame buffer color attachment
	uint32_t handle = sl::RenderCore::GetMainFramebuffer()->GetAttachmentHandle(0);
	ImGui::Image((void *)(uint64_t)handle, ImGui::GetContentRegionAvail(), ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

	auto &camera = sl::ECSWorld::GetEditorCameraComponent();

	// ImGuizmo
	ShowImGuizmoOrientation();
	if (m_imguizmoMode >= 0 && m_selectedEntity)
	{
		ShowImGuizmoTransform();
	}

	if (!ImGui::IsWindowHovered() || camera.IsUsing() ||
		ImGui::IsMouseDragging(ImGuiMouseButton_Left) ||
		ImGuizmo::IsUsing())
	{
		ImGui::End();
		return;
	}

	// Set camera controller mode or mouse pick entity,
	// which related to mouse position so we cant just call them inside OnEnvent.
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		camera.m_controllerMode = sl::CameraControllerMode::FPS;
	}
	else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		if (ImGui::IsKeyDown(ImGuiKey_LeftAlt))
		{
			camera.m_controllerMode = sl::CameraControllerMode::Editor;
		}
		else
		{
			MousePick();
		}
	}

	ImGui::End();
}

bool ImGuiLayer::OnKeyPressed(sl::KeyPressEvent& event)
{
	if (sl::ECSWorld::GetEditorCameraComponent().IsUsing() || ImGuizmo::IsUsing())
	{
		return false;
	}

	auto key = event.GetKey();
	switch (key)
	{
		case SL_KEY_Q:
		{
			m_imguizmoMode = -1;
			break;
		}
		case SL_KEY_W:
		{
			m_imguizmoMode = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case SL_KEY_E:
		{
			m_imguizmoMode = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case SL_KEY_R:
		{
			m_imguizmoMode = ImGuizmo::OPERATION::SCALE;
			break;
		}
		default:
		{
			break;
		}
	}

	return false;
}

// For ImGuiLayer::m_dockSpaceFlag
static_assert(std::is_same_v<ImGuiDockNodeFlags, int>);
static_assert(ImGuiDockNodeFlags_None == 0);

// For ImGuiLayer::m_imguizmoMode
static_assert(std::is_same_v<std::underlying_type_t<ImGuizmo::OPERATION>, int>);
static_assert(ImGuizmo::OPERATION::TRANSLATE == 7);

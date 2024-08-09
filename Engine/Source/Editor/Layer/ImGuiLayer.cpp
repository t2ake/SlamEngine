#include "ImGuiLayer.h"

#include "Core//Path.hpp"
#include "Core/EnumOf.hpp"
#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/SceneViewportEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "Panel/ScrollingBuffer.h"
#include "RenderCore/RenderCore.h"
#include "Resource/Font.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneSerializer.h"
#include "Window/Input.h"

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

SL_FORCEINLINE float GetTitleBarSize()
{
	// Can't change in runtime.
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

bool AlignButton(const char *label, float align = 0.5f, float customOffset = 0.0f)
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
	dispatcher.Dispatch<sl::MouseButtonPressEvent>(BIND_EVENT_CALLBACK(ImGuiLayer::OnMouseButtonPress));
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

	if (m_selectedEntity == 0) {}
	if (0 == m_selectedEntity) {}

	if (m_selectedEntity == entt::entity{ 0 }) {}
	if (entt::entity{ 0 } == m_selectedEntity) {}

	if (m_selectedEntity == sl::Entity{ 0 }) {}
	if (sl::Entity{ 0 } == m_selectedEntity) {}

	if (m_selectedEntity.IsValid()) {}
	if (!m_selectedEntity.IsValid()) {}
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
	// Display on the upper left corner.
	constexpr float ToolOverlayOffset = 10.0f;
	ImGui::SetNextWindowPos(ImVec2{
		(float)m_sceneViewportWindowPosX + ToolOverlayOffset,
		(float)m_sceneViewportWindowPosY + ToolOverlayOffset + GetTitleBarSize() });

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
	ImGui::Begin("Tools", nullptr, OverlayButtonFlags);
	ImGui::PopStyleVar();

	constexpr std::array<int, 4> Operations =
	{
		-1, // Don't use ImGuizmo tramsform.
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

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_imguizmoMode == op ? 1.0f : 0.5f);
		if (ImGui::Button(Icons[index], ImVec2{ 32.0f, 32.0f }))
		{
			m_imguizmoMode = op;
		}
		ImGui::PopStyleVar();
	};

	for (size_t i = 0; i < 4; ++i)
	{
		SelectableButton(i);
	}

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
		// Waiting for programme to be stable.
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
				&s_coastBuffer.m_datas[0].x,
				&s_coastBuffer.m_datas[0].y,
				s_coastBuffer.m_datas.size(),
				0, 0,
				s_coastBuffer.m_offset,
				2 * sizeof(float));

			ImPlot::PlotLine("FPS",
				&s_fpsBuffer.m_datas[0].x,
				&s_fpsBuffer.m_datas[0].y,
				s_fpsBuffer.m_datas.size(),
				0,
				s_fpsBuffer.m_offset,
				2 * sizeof(float));
		}
		ImPlot::EndPlot();
	}

	ImGui::End();
}

void ImGuiLayer::ShowLog()
{
	// 0011 1111, which means not filtering anything.
	constexpr uint8_t FullLevelFilter = 0x3f;
	static uint8_t s_levelFilter = FullLevelFilter;
	static ImGuiTextFilter s_textFilter;

	constexpr ImVec4 TraceColor    { 1.0f, 1.0f, 1.0f, 1.0f };
	constexpr ImVec4 DebugColor    { 0.0f, 0.0f, 1.0f, 1.0f };
	constexpr ImVec4 InfoColor     { 0.0f, 1.0f, 0.0f, 1.0f };
	constexpr ImVec4 WarnColor     { 1.0f, 1.0f, 0.0f, 1.0f };
	constexpr ImVec4 ErrorColor    { 1.0f, 0.0f, 0.0f, 1.0f };
	constexpr ImVec4 CriticalColor { 1.0f, 0.0f, 1.0f, 1.0f };

	auto LogLevelToColor = [](sl::LogLevel level)
	{
		switch (level)
		{
			case sl::LogLevel::Trace:
			{
				return TraceColor;
			}
			case sl::LogLevel::Debug:
			{
				return DebugColor;
			}
			case sl::LogLevel::Info:
			{
				return InfoColor;
			}
			case sl::LogLevel::Warn:
			{
				return WarnColor;
			}
			case sl::LogLevel::Error:
			{
				return ErrorColor;
			}
			case sl::LogLevel::Critical:
			{
				return CriticalColor;
			}
			default:
			{
				return ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
			}
		}
	};

	auto LevelToIcon = [](sl::LogLevel level)
	{
		switch (level)
		{
			case sl::LogLevel::Trace:
			{
				return "T";
			}
			case sl::LogLevel::Debug:
			{
				return "D";
			}
			case sl::LogLevel::Info:
			{
				return "I";
			}
			case sl::LogLevel::Warn:
			{
				return "W";
			}
			case sl::LogLevel::Error:
			{
				return "E";
			}
			case sl::LogLevel::Critical:
			{
				return "C";
			}
			default:
			{
				return "?";
			}
		}
	};

	// Show log level filter button.
	auto LevelButton = [&](sl::LogLevel level)
	{
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, LogLevelToColor(level));
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, (s_levelFilter & (uint8_t)level) ? 1.0f : 0.5f);
		if (ImGui::Button(LevelToIcon(level)))
		{
			s_levelFilter ^= (uint8_t)level;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	};

	ImGui::Begin("Log");
	RightClickFocus();
	 
	if (ImGui::Button("Clear"))
	{
		sl::Log::GetLogInfos().clear();
	}

	ImGui::SameLine();
	if (ImGui::Button("Copy"))
	{
		ImGui::LogToClipboard();
	}

	LevelButton(sl::LogLevel::Trace);
	LevelButton(sl::LogLevel::Debug);
	LevelButton(sl::LogLevel::Info);
	LevelButton(sl::LogLevel::Warn);
	LevelButton(sl::LogLevel::Error);
	LevelButton(sl::LogLevel::Critical);

	ImGui::SameLine();
	s_textFilter.Draw("##TextFilter", -ImGui::GetStyle().ScrollbarSize);

	ImGui::Separator();

	auto &logInfos = sl::Log::GetLogInfos();
	if (ImGui::BeginChild("ScrollingLog", ImVec2(0, 0), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		// If any filter is active.
		if (s_levelFilter < FullLevelFilter || s_textFilter.IsActive())
		{
			for (size_t i = 0; i < logInfos.size(); ++i)
			{
				const auto &info = logInfos[i];
				if ((s_levelFilter & (uint8_t)info.m_level) && s_textFilter.PassFilter(info.m_text.data()))
				{
					ImGui::PushStyleColor(ImGuiCol_Text, LogLevelToColor(info.m_level));
					ImGui::TextUnformatted(info.m_text.data());
					ImGui::PopStyleColor();
				}
			}
		}
		else // Without any filter
		{
			ImGuiListClipper clipper;
			clipper.Begin((int)logInfos.size());
			while (clipper.Step())
			{
				for (size_t i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
				{
					const auto &info = logInfos[i];
					ImGui::PushStyleColor(ImGuiCol_Text, LogLevelToColor(info.m_level));
					ImGui::TextUnformatted(info.m_text.data());
					ImGui::PopStyleColor();
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		// Auto scrolling
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.0f);
		}
	}
	
	ImGui::EndChild();
	ImGui::End();
}

void ImGuiLayer::ShowEntityList()
{
	ImGui::Begin("Entity List");
	RightClickFocus();

	bool isItemClicked = false;

	// Each entity holds a TagComponent.
	auto allEntityView = sl::ECSWorld::GetRegistry().view<sl::TagComponent>();
	for (auto entity : allEntityView)
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

		auto &tag = allEntityView.get<sl::TagComponent>(entity);
		bool nodeOpen = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, treeNodeFlag, tag.m_name.c_str());

		// Left click to select entity.
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			m_selectedEntity = entity;
		}

		// Right click to open an entity popup.
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			isItemClicked = true;
			ImGui::OpenPopup("EntityPopup");
		}
		if (ImGui::BeginPopup("EntityPopup"))
		{
			if (ImGui::MenuItem("Destory Entity"))
			{
				if (m_selectedEntity == entity)
				{
					m_selectedEntity.Reset();
				}
				sl::Entity{ entity }.Destroy();
			}
			ImGui::EndPopup();
		}

		if (nodeOpen)
		{
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	if (!isItemClicked && ImGui::IsWindowHovered())
	{
		// Left click on an enpty space to clear selected entity.
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_selectedEntity.Reset();
		}
		// Right click on an enpty space to create a new entity.
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("CreateNewEntityPopup");
		}
	}
	if (ImGui::BeginPopup("CreateNewEntityPopup"))
	{
		if (ImGui::MenuItem("Creat Empty Entity"))
		{
			sl::ECSWorld::CreateEntity();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}

void ImGuiLayer::ShowAssetBrowser()
{
	ImGui::Begin("Asset Browser");

	// Disable the back button if the current path reaches the outermost path.
	std::string crtPath = m_assetBrowserCrtPath.generic_string();
	const bool backButtonDisabled = !sl::Path::Contain(sl::Path::AssetPath, crtPath);
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

	if (columnCount < 1)
	{
		ImGui::End();
		return;
	}
	
	SL_ASSERT(columnCount > 0);
	ImGui::Columns(columnCount, "AssetBrowserColums", false);

	uint32_t columnIndex = 0;
	for (const auto &it : std::filesystem::directory_iterator(m_assetBrowserCrtPath))
	{
		columnIndex = columnIndex >= columnCount ? 0 : columnIndex;
		if (columnCount > 1)
		{
			// WHY: ImGui::SetColumnWidth just unhappy with displaying only one column.
			ImGui::SetColumnWidth(columnIndex++, columnSize);
		}

		std::string fileName = it.path().filename().generic_string();
		ImGui::PushID(fileName.c_str());

		const bool isDirectory = it.is_directory();
		auto *pTextureResource = isDirectory ?
			sl::ResourceManager::GetTextureResource(m_folderIconName) :
			sl::ResourceManager::GetTextureResource(m_fileIconName);

		if (pTextureResource->IsReady())
		{
			ImGui::ImageButton(fileName.c_str(), (ImTextureID)(uint64_t)pTextureResource->GetTexture()->GetHandle(),
				ButtonSize, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
			
			if (isDirectory && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_assetBrowserCrtPath = it.path();
			}

			ImGui::TextWrapped(fileName.c_str());
		}

		ImGui::PopID();
		ImGui::NextColumn();
	}

	ImGui::Columns(1);
	ImGui::End();
}

template<class T, class Fun>
void ImGuiLayer::DrawComponent(const char *label, Fun uiFunction)
{
	T *pComponent = m_selectedEntity.TryGetComponent<T>();
	if (!pComponent)
	{
		return;
	}

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
		// WHY: Don't know why (WindowPadding.x / 2) just works perfect here.
		ImGui::OpenPopup("ComponentPopup");
	}
	ImGui::PopStyleColor();

	bool removeComponent = false;
	if (ImGui::BeginPopup("ComponentPopup"))
	{
		// C++ 20 "requires" allows us to avoid forcing all composnets to implement a Reset function.
		if constexpr (requires{ pComponent->Reset(); })
		{
			if (ImGui::MenuItem("Reset Component"))
			{
				pComponent->Reset();

				// The camera entity will be affected by TransformComponent.
				if constexpr (std::is_same_v<T, sl::TransformComponent>)
				{
					if (auto pCamera = m_selectedEntity.TryGetComponent<sl::CameraComponent>(); pCamera)
					{
						pCamera->m_isDirty = true;
					}
				}
			}
		}

		// We don't allow to remove TagComponent and TransformComponent.
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

void ImGuiLayer::StartWithText(std::string_view text)
{
	static sl::Entity s_crtEntity;
	if (s_crtEntity != m_selectedEntity)
	{
		// ImGui::CalcTextSize("Rotation").x == 56.0f
		// ImGui::CalcTextSize("Position").x == 56.0f
		// Just a little trick to avoid Tag Component flickering when it's rendered the first time,
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

	if (!m_selectedEntity.IsValid())
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
			"ImGui ensure that InputText() returns a null-terminated character array,"
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
			// If we select camera entitiy.
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
		ImGui::OpenPopup("AddComponentPopup");
	}
	if (ImGui::BeginPopup("AddComponentPopup"))
	{
		AddComponent<sl::CameraComponent>("Camera");
		AddComponent<sl::CornerstoneComponent>("Cornerstone");
		AddComponent<sl::RenderingComponent>("Rendering");
		ImGui::EndPopup();
	}

	ImGui::End();
}

void ImGuiLayer::ShowImGuizmoOrientation()
{
	// Display a cube on the upper right corner.
	constexpr float Length = 100.0f;
	ImVec2 pos = ImVec2{
		(float)m_sceneViewportWindowPosX + (float)m_sceneViewportSizeX - Length,
		(float)m_sceneViewportWindowPosY + GetTitleBarSize() };

	auto view = sl::ECSWorld::GetEditorCameraComponent().GetView();
	ImGuizmo::ViewManipulate(glm::value_ptr(view), Length, pos, ImVec2{ Length , Length }, 0);
}

void ImGuiLayer::ShowImGuizmoTransform()
{
	if (m_selectedEntity == sl::ECSWorld::GetEditorCameraEntity())
	{
		return;
	}

	// Disable ImGuizmo when camera is using.
	auto &camera = sl::ECSWorld::GetEditorCameraComponent();
	ImGuizmo::Enable(!camera.IsUsing());

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetOrthographic(camera.m_projectionType == sl::ProjectionType::Orthographic ? true : false);
	ImGuizmo::AllowAxisFlip(false);
	ImGuizmo::SetRect((float)m_sceneViewportWindowPosX, (float)m_sceneViewportWindowPosY + GetTitleBarSize(),
		(float)m_sceneViewportSizeX, (float)m_sceneViewportSizeY);

	const glm::mat4 &view = camera.GetView();
	const glm::mat4 &projection = camera.GetProjection();

	auto &transform = m_selectedEntity.GetComponent<sl::TransformComponent>();
	glm::mat4 manipulatedTransform = transform.GetTransform();

	bool useSnap = ImGui::IsKeyDown(ImGuiKey_LeftCtrl);
	float snap = (m_imguizmoMode == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f);
	float snaps[] = { snap, snap, snap };

	ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
		(ImGuizmo::OPERATION)m_imguizmoMode, ImGuizmo::LOCAL, glm::value_ptr(manipulatedTransform),
		nullptr, useSnap ? snaps : nullptr);

	if(ImGuizmo::IsUsing())
	{
		// Decompose transform mat to position, rotation and scale.
		glm::vec3 newPosition, newRotation, newScale;
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(manipulatedTransform),
			glm::value_ptr(newPosition), glm::value_ptr(newRotation), glm::value_ptr(newScale));

		transform.m_position = newPosition;
		transform.m_rotation += glm::radians(newRotation) - transform.m_rotation;
		transform.m_scale = newScale;
	}
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
	if (m_sceneViewportSizeX != crtSceneViewportSizeX || m_sceneViewportSizeY != crtSceneViewportSizeY)
	{
		m_sceneViewportSizeX = crtSceneViewportSizeX;
		m_sceneViewportSizeY = crtSceneViewportSizeY;

		sl::SceneViewportResizeEvent event{ m_sceneViewportSizeX , m_sceneViewportSizeY };
		m_eventCallback(event);
	}

	// Draw main frame buffer color attachment
	uint32_t handle = sl::RenderCore::GetMainFramebuffer()->GetAttachmentHandle(0);
	ImGui::Image((void *)(uint64_t)handle, ImGui::GetContentRegionAvail(), ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

	// ImGuizmo
	ShowImGuizmoOrientation();
	if (m_imguizmoMode >= 0 && m_selectedEntity.IsValid())
	{
		ShowImGuizmoTransform();
	}

	if (sl::ECSWorld::GetEditorCameraComponent().IsUsing() ||
		!ImGui::IsWindowHovered() || ImGuizmo::IsUsing() ||
		ImGui::IsMouseDragging(ImGuiMouseButton_Left) ||
		ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		m_isMouseFreeInSceneView = false;

	}
	else
	{
		m_isMouseFreeInSceneView = true;
	}

	ImGui::End();
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

	// We clear the Entity ID buffer by -1 every frame in RendererLayer.
	if (entityID < 0)
	{
		m_selectedEntity.Reset();
		return;
	}

	sl::Entity crtEntity{ (uint32_t)entityID };
	if (m_selectedEntity == crtEntity)
	{
		return;
	}

	SL_LOG_TRACE("Select entity, ID: {}, Name: \"{}\", Mouse position: ({}, {})",
		entityID, crtEntity.GetComponent<sl::TagComponent>().m_name.c_str(),
		mouseLocalPosX, mouseLocalPosY);

	m_selectedEntity = crtEntity;
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

bool ImGuiLayer::OnMouseButtonPress(sl::MouseButtonPressEvent &event)
{
	if (!m_isMouseFreeInSceneView)
	{
		return false;
	}

	auto &camera = sl::ECSWorld::GetEditorCameraComponent();
	if (event.GetButton() == SL_MOUSE_BUTTON_LEFT)
	{
		if (sl::Input::IsKeyPressed(SL_KEY_LEFT_ALT))
		{
			camera.m_controllerMode = sl::CameraControllerMode::Editor;
		}
		else if (!ImGuizmo::IsOver())
		{
			MousePick();
		}
	}
	else if (event.GetButton() == SL_MOUSE_BUTTON_RIGHT)
	{
		camera.m_controllerMode = sl::CameraControllerMode::FPS;
	}

	return false;
}

// For ImGuiLayer::m_dockSpaceFlag
static_assert(std::is_same_v<ImGuiDockNodeFlags, int>);
static_assert(ImGuiDockNodeFlags_None == 0);

// For ImGuiLayer::m_imguizmoMode
static_assert(std::is_same_v<std::underlying_type_t<ImGuizmo::OPERATION>, int>);
static_assert(ImGuizmo::OPERATION::TRANSLATE == 7);

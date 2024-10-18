#include "ImGuiLayer.h"

#include "Core//Path.hpp"
#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/SceneViewportEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/IconsMaterialSymbols.h"
#include "ImGui/ImGuiContext.h"
#include "RenderCore/RenderCore.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneSerializer.h"
#include "Utils/EnumOf.hpp"
#include "Utils/ProfilerCPU.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>
#include <implot/implot.h>
#include <nameof/nameof.hpp>

namespace
{

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

SL_FORCEINLINE float GetDPIFactor()
{
	// TEMPORARY
	return ImGui::GetFontSize();
}

float GetTitleBarSize()
{
	return ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
}

void RightClickFocus()
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		ImGui::SetWindowFocus();
	}
}

void AlignNextWidget(const char *label, float align = 0.5f, float customOffset = 0.0f)
{
	float size = ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;
	float offset = (avail - size) * align;
	if (offset > 0.0f)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset + customOffset);
	}
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

// From ImPlot
struct ScrollingBuffer
{
	ScrollingBuffer(int max_size = 1024)
	{
		m_maxSize = max_size;
		m_offset = 0;
		m_datas.reserve(m_maxSize);
	}
	void AddPoint(float x, float y)
	{
		if (m_datas.size() < m_maxSize)
		{
			m_datas.push_back(ImVec2(x, y));
		}
		else
		{
			m_datas[m_offset] = ImVec2(x, y);
			m_offset = (m_offset + 1) % m_maxSize;
		}
	}
	void Erase()
	{
		if (m_datas.size() > 0)
		{
			m_datas.shrink(0);
			m_offset = 0;
		}
	}

	int m_maxSize;
	int m_offset;
	ImVector<ImVec2> m_datas;
};

} // namespace

ImGuiLayer::ImGuiLayer()
{
	SL_PROFILE;

	m_dockSpaceFlag |= ImGuiDockNodeFlags_NoUndocking;
	m_selectedEntity = sl::ECSWorld::GetMainCameraEntity();
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
	SL_PROFILE;

	sl::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<sl::KeyPressEvent>(BIND_EVENT_CALLBACK(ImGuiLayer::OnKeyPressed));
	dispatcher.Dispatch<sl::KeyReleaseEvent>(BIND_EVENT_CALLBACK(ImGuiLayer::OnKeyRelease));
	dispatcher.Dispatch<sl::MouseButtonPressEvent>(BIND_EVENT_CALLBACK(ImGuiLayer::OnMouseButtonPress));
	dispatcher.Dispatch<sl::MouseButtonReleaseEvent>(BIND_EVENT_CALLBACK(ImGuiLayer::OnMouseButtonRelease));
}

void ImGuiLayer::BeginFrame()
{

}

void ImGuiLayer::OnUpdate(float deltaTime)
{
	SL_PROFILE;

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
	SL_PROFILE;

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
	if (m_debugImGuizmoStatus)
	{
		ImGui::Begin("ImGuizmo Status", &m_debugImGuizmoStatus);

		ImGui::Text("IsOver: ");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsOver() ? "true" : "fasle");
		ImGui::Text("IsUsing: ");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsUsing() ? "true" : "fasle");
		ImGui::Text("IsUsingAny: ");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsUsingAny() ? "true" : "fasle");

		ImGui::End();
	}
}

void ImGuiLayer::ShowToolOverlay()
{
	SL_PROFILE;

	constexpr size_t ToolCount = 4;
	constexpr std::array<int, ToolCount> Operations =
	{
		-1, // No ImGuizmo tramsform.
		ImGuizmo::OPERATION::TRANSLATE,
		ImGuizmo::OPERATION::ROTATE,
		ImGuizmo::OPERATION::SCALE,
	};
	constexpr std::array<const char *, ToolCount> Icons =
	{
		ICON_MS_ARROW_SELECTOR_TOOL, ICON_MS_DRAG_PAN, ICON_MS_CACHED, ICON_MS_ZOOM_OUT_MAP,
	};

	auto SelectableButton = [this](size_t index)
	{
		const int op = Operations.at(index);

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_imguizmoMode == op ? 1.0f : 0.5f);
		if (ImGui::Button(Icons.at(index), ImVec2{ GetDPIFactor() * 2.0f, GetDPIFactor() * 2.0f }))
		{
			m_imguizmoMode = op;
		}
		ImGui::PopStyleVar();
	};

	// Display on the upper left corner.
	ImGui::SetNextWindowPos(ImVec2{
		(float)m_sceneViewportWindowPosX + GetDPIFactor() * 0.5f,
		(float)m_sceneViewportWindowPosY + GetDPIFactor() * 0.5f + GetTitleBarSize() });

	ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);

	for (size_t i = 0; i < ToolCount; ++i)
	{
		SelectableButton(i);
	}

	ImGui::End();
}

void ImGuiLayer::ShowMenuBar()
{
	SL_PROFILE;

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
		ImGui::MenuItem("ImGuizmo Status", "", &m_debugImGuizmoStatus);
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void ImGuiLayer::ShowInfo(float deltaTime)
{
	SL_PROFILE;

	ImGui::Begin("Info");
	RightClickFocus();

	// 1. Infos
	ImGui::Text("Backend: %s", nameof::nameof_enum(sl::RenderCore::GetBackend()).data());
	ImGui::Separator();

	// 2. FPS
	static float s_sumTime = 0.0f; // Stores in millisecond.
	static float s_deltaTimeMultiplier = 10.0f;
	static ScrollingBuffer s_coastBuffer;
	static ScrollingBuffer s_fpsBuffer;

	constexpr float History = 500.0f;
	constexpr float Delay = 500.0f;

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
	SL_PROFILE;

	// 0011 1111, which means not filtering anything.
	constexpr uint8_t FullLevelFilter = 0x3f;
	static uint8_t s_levelFilter = FullLevelFilter;
	static ImGuiTextFilter s_textFilter;

	constexpr ImVec4 TraceColor { 1.0f, 1.0f, 1.0f, 1.0f };
	constexpr ImVec4 DebugColor { 0.1f, 0.5f, 0.9f, 1.0f };
	constexpr ImVec4 InfoColor  { 0.1f, 0.9f, 0.1f, 1.0f };
	constexpr ImVec4 WarnColor  { 0.9f, 0.8f, 0.1f, 1.0f };
	constexpr ImVec4 ErrorColor { 0.9f, 0.1f, 0.1f, 1.0f };
	constexpr ImVec4 FatalColor { 1.0f, 0.1f, 1.0f, 1.0f };

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
			case sl::LogLevel::Fatal:
			{
				return FatalColor;
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
				return ICON_MS_NOTES;
			}
			case sl::LogLevel::Debug:
			{
				return ICON_MS_BUILD;
			}
			case sl::LogLevel::Info:
			{
				return ICON_MS_CHAT;
			}
			case sl::LogLevel::Warn:
			{
				return ICON_MS_WARNING;
			}
			case sl::LogLevel::Error:
			{
				return ICON_MS_CANCEL;
			}
			case sl::LogLevel::Fatal:
			{
				return ICON_MS_BUG_REPORT;
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
	LevelButton(sl::LogLevel::Fatal);

	ImGui::SameLine();
	s_textFilter.Draw("##TextFilter", -ImGui::GetStyle().ScrollbarSize);

	ImGui::Separator();

	// TODO: Show time and level on log panel.
	auto &logInfos = sl::Log::GetLogInfos();
	if (ImGui::BeginChild("LogTexts", ImVec2{ 0.0f, 0.0f }, ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

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
		else // Without any filter.
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

		// Auto scrolling.
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
	SL_PROFILE;

	ImGui::Begin("Entity List");
	RightClickFocus();

	bool isEntityRightClicked = false;

	// Each entity holds a TagComponent.
	auto allEntityView = sl::ECSWorld::GetRegistry().view<sl::TagComponent>();
	for (auto entity : allEntityView)
	{
		ImGui::PushID((void *)(uint64_t)(uint32_t)entity);

		// TODO: Hierarchy
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
			isEntityRightClicked = true;
			ImGui::OpenPopup("EntityPopup");
		}
		if (ImGui::BeginPopup("EntityPopup"))
		{
			if (ImGui::MenuItem("Destory Entity"))
			{
				bool canDestroy = true;
				sl::Entity slEntity{ entity };

				// It is not allowed to destroy entity with Cornerstone coponent.
				if (slEntity.TryGetComponents<sl::CornerstoneComponent>())
				{
					canDestroy = false;
					SL_LOG_WARN("Can not destroy entity \"{}\" with Cornerstone component!",
						slEntity.GetComponents<sl::TagComponent>().m_name.c_str());
				}
				// It is not allowed to destroy entity that is the main camera.
				if (auto pCameraComponent = slEntity.TryGetComponents<sl::CameraComponent>();
					pCameraComponent && pCameraComponent->m_isMainCamera)
				{
					canDestroy = false;
					SL_LOG_WARN("Can not destroy entity \"{}\" as main camera!",
						slEntity.GetComponents<sl::TagComponent>().m_name.c_str());
				}

				if (canDestroy)
				{
					if (m_selectedEntity == entity)
					{
						m_selectedEntity.Reset();
					}
					slEntity.Destroy();
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

	if (!isEntityRightClicked && ImGui::IsWindowHovered())
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
	SL_PROFILE;

	ImGui::Begin("Asset Browser");

	// Disable the back button if the current path reaches the outermost path.
	std::string crtPath = m_assetBrowserCrtPath.generic_string();
	const bool backButtonDisabled = !sl::Path::Contain(sl::Path::AssetPath, crtPath);
	if (backButtonDisabled)
	{
		ImGui::BeginDisabled();
	}
	if (ImGui::Button(ICON_MS_ARROW_BACK))
	{
		m_assetBrowserCrtPath = m_assetBrowserCrtPath.parent_path();
	}
	if (backButtonDisabled)
	{
		ImGui::EndDisabled();
	}

	// Display the current path.
	ImGui::SameLine();
	ImGui::Text(crtPath.c_str());

	// Select item size.
	static size_t s_itemSizeIndex = 2;
	constexpr size_t SizeCount = 5;
	float DPIFactor = GetDPIFactor();
	const std::array<float, SizeCount> BasicColumSizes =
	{
		DPIFactor * 2.0f, DPIFactor * 3.0f, DPIFactor * 4.0f, DPIFactor * 5.0f, DPIFactor * 6.0f,
	};
	constexpr std::array<const char *, SizeCount> SizeNames =
	{
		"Very Small", "Small", "Medium", "Large", "Very Large",
	};
	constexpr std::array<uint8_t, SizeCount> NameDisplayLines =
	{
		1, 2, 3, 4, 5,
	};

	ImGui::SameLine();
	AlignNextWidget(SizeNames.at(s_itemSizeIndex), 1.0f, -25.0f);
	// 25.0f is approximately the size of the triangle of the combo widget.
	if (ImGui::BeginCombo("##ItemSizeCombo", SizeNames.at(s_itemSizeIndex), ImGuiComboFlags_WidthFitPreview))
	{
		for (size_t i = 0; i < SizeCount; ++i)
		{
			if (ImGui::Selectable(SizeNames.at(i), i == s_itemSizeIndex))
			{
				s_itemSizeIndex = i;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Separator();

	float available = ImGui::GetContentRegionAvail().x;
	float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
	ImVec2 framePadding = ImGui::GetStyle().FramePadding;

	// To avoid scroll bar flickering.
	if (ImGui::GetScrollMaxY() < 1.0f)
	{
		available -= ImGui::GetStyle().ScrollbarSize;
	}

	if (available <= 0.0f)
	{
		ImGui::End();
		return;
	}

	available += itemSpacing;
	uint32_t columCount = (uint32_t)available / (uint32_t)BasicColumSizes.at(s_itemSizeIndex);
	if (columCount < 1)
	{
		columCount = 1;
	}

	float columSize = std::floor(available / (float)columCount);
	float filledItemSize = columSize - itemSpacing - 1.0f;
	
	// Show files and folders under current path.
	uint32_t columIndex = 0;
	ImGui::Columns(columCount, "##AssetBrowserColums", false);
	for (const auto &it : std::filesystem::directory_iterator(m_assetBrowserCrtPath))
	{
		if (columCount > 1)
		{
			if (columIndex >= columCount)
			{
				columIndex = 0;
			}
			ImGui::SetColumnWidth(columIndex++, columSize);
		}

		std::string fileName = it.path().filename().generic_string();
		ImGui::PushID(fileName.c_str());

		const bool isDirectory = it.is_directory();
		auto *pTextureResource = isDirectory ?
			sl::ResourceManager::GetTextureResource(m_folderIconName) :
			sl::ResourceManager::GetTextureResource(m_fileIconName);

		if (pTextureResource->IsReady())
		{
			// ImageButton will adds `style.FramePadding * 2.0f` to provided size.
			ImGui::ImageButton("##Icon", (ImTextureID)(uint64_t)pTextureResource->GetTexture()->GetHandle(),
				ImVec2{ filledItemSize - framePadding.x * 2.0f, filledItemSize - framePadding.y * 2.0f },
				ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

			// Open the folder when double clicking on the folder.
			if (isDirectory && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_assetBrowserCrtPath = it.path();
			}
			
			// Limit the number of lines displayed for a file or folder name.
			ImGui::BeginChild(
				"##Text",
				ImVec2{ filledItemSize, ImGui::GetFontSize() * (float)NameDisplayLines.at(s_itemSizeIndex) },
				ImGuiChildFlags_None,
				ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDecoration);
			ImGui::TextWrapped(fileName.c_str());
			ImGui::EndChild();
		}

		ImGui::PopID();
		ImGui::NextColumn();
	}

	ImGui::Columns(1);
	ImGui::End();
}

template<class T>
void ImGuiLayer::DrawComponent(const char *label, auto drawParameters)
{
	static_assert(requires{ drawParameters(m_selectedEntity.TryGetComponents<T>()); });

	T *pComponent = m_selectedEntity.TryGetComponents<T>();
	if (!pComponent)
	{
		return;
	}

	ImGui::PushID(nameof::nameof_type<T>().data());

	// Draw tree node.
	ImGui::PushFont(sl::ImGuiContext::GetBoldFont());
	bool componentTreeOpen = ImGui::TreeNodeEx(label, DefaultTreeFlags, label);
	ImGui::PopFont();

	// Draw component menu button.
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
	AlignNextWidget(ICON_MS_MORE_VERT, 1.0f, ImGui::GetStyle().WindowPadding.x * 0.5f);
	if(ImGui::Button(ICON_MS_MORE_VERT))
	{
		ImGui::OpenPopup("ComponentPopup");
	}
	ImGui::PopStyleColor();

	bool removeComponent = false;
	if (ImGui::BeginPopup("ComponentPopup"))
	{
		// C++ 20 `requires` allows us to avoid forcing all composnets to implement a `Reset` function.
		if constexpr (requires{ pComponent->Reset(); })
		{
			if (ImGui::MenuItem("Reset Component"))
			{
				pComponent->Reset();

				// The camera entity will be affected by Transform component.
				if constexpr (std::is_same_v<T, sl::TransformComponent>)
				{
					if (auto pCamera = m_selectedEntity.TryGetComponents<sl::CameraComponent>(); pCamera)
					{
						pCamera->m_isDirty = true;
					}
				}
			}
		}

		bool canRemove = true;
		// It is not allowed to remove Tag and Transform component.
		if constexpr (std::is_same_v<T, sl::TagComponent> || std::is_same_v<T, sl::TransformComponent>)
		{
			canRemove = false;
		}
		// It is not allowed to remove Camera component if it is a main camera.
		else if (std::is_same_v<T, sl::CameraComponent>)
		{
			if (m_selectedEntity.GetComponents<sl::CameraComponent>().m_isMainCamera)
			{
				canRemove = false;
			}
		}

		if (canRemove && ImGui::MenuItem("Remove Component"))
		{
			m_selectedEntity.RemoveComponent<T>();
			m_maxTextSize = 56.0f;
			removeComponent = true;
		}

		ImGui::EndPopup();
	}

	// Draw component specific parameters ui.
	if (componentTreeOpen && !removeComponent)
	{
		drawParameters(pComponent);
	}

	ImGui::Separator();
	ImGui::PopID();
}

template<class T>
void ImGuiLayer::AddComponentMenuItem(const char *label)
{
	static_assert(requires{ m_selectedEntity.TryGetComponents<T>(); });

	if (ImGui::MenuItem(label))
	{
		if (!m_selectedEntity.TryGetComponents<T>())
		{
			m_selectedEntity.AddComponent<T>();
		}
		else
		{
			SL_LOG_WARN("Entity \"{}\" already has component \"{}\"",
				m_selectedEntity.GetComponents<sl::TagComponent>().m_name, label);
		}
	}
}

void ImGuiLayer::StartWithText(std::string_view text)
{
	static sl::Entity s_crtEntity;
	if (s_crtEntity != m_selectedEntity)
	{
		// `ImGui::CalcTextSize("Rotation").x == 56.0f`
		// `ImGui::CalcTextSize("Position").x == 56.0f`
		// Just a little trick to avoid Tag component flickering when it is rendered the first time,
		// as we known every entity must hold both Tag and Transform component.
		m_maxTextSize = 56.0f;
		s_crtEntity = m_selectedEntity;
	}

	float crtTextSize = ImGui::CalcTextSize(text.data()).x;
	if (crtTextSize > m_maxTextSize)
	{
		m_maxTextSize = crtTextSize;
	}

	float offset = ImGui::GetStyle().IndentSpacing + GetDPIFactor();
	ImGui::SetCursorPosX(offset);
	ImGui::AlignTextToFramePadding();
	ImGui::Text(text.data());

	ImGui::SameLine(m_maxTextSize + offset + GetDPIFactor());
	ImGui::SetNextItemWidth(-GetDPIFactor());
}

void ImGuiLayer::ShowDetails()
{
	SL_PROFILE;

	ImGui::Begin("Details");
	RightClickFocus();

	if (!m_selectedEntity.IsValid())
	{
		ImGui::End();
		return;
	}

	bool dragWidgetActivated = false;
	bool dragWidgetDeactivated = false;
	auto CheckDragWidgetState = [&dragWidgetActivated, &dragWidgetDeactivated]()
	{
		dragWidgetActivated |= ImGui::IsItemActivated();
		dragWidgetDeactivated |= ImGui::IsItemDeactivated();
	};

	// Draw Tag component.
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

	// Draw Transform component.
	DrawComponent<sl::TransformComponent>("Transform", [this, &CheckDragWidgetState](sl::TransformComponent *pComponent)
	{
		bool cameraMayBeDirty = false;

		glm::vec3 &position = pComponent->m_position;
		StartWithText("Position");
		if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.1f))
		{
			cameraMayBeDirty = true;
		}
		CheckDragWidgetState();

		glm::vec3 ratationDegrees = RotationModAndRepeat(pComponent->GetRotationDegrees());
		StartWithText("Rotation");
		if (ImGui::DragFloat3("##Rotation", glm::value_ptr(ratationDegrees), 0.1f))
		{
			pComponent->SetRotationDegrees(ratationDegrees);
			cameraMayBeDirty = true;
		}
		CheckDragWidgetState();

		glm::vec3 &scale = pComponent->m_scale;
		StartWithText("Scale");
		ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f);
		CheckDragWidgetState();

		if (cameraMayBeDirty)
		{
			// If we select camera entitiy.
			if (auto *pCamera = m_selectedEntity.TryGetComponents<sl::CameraComponent>(); pCamera)
			{
				pCamera->m_isDirty = true; 
			}
		}
	});

	// Draw Camera component.
	DrawComponent<sl::CameraComponent>("Camera", [this, &CheckDragWidgetState](sl::CameraComponent *pComponent)
	{
		StartWithText("Main Camera");
		bool isMainCamera = pComponent->m_isMainCamera;
		if(ImGui::Checkbox("##MainCameraCheckBox", &isMainCamera))
		{
			if (isMainCamera)
			{
				auto view = sl::ECSWorld::GetRegistry().group<sl::CameraComponent>();
				for (auto entity : view)
				{
					view.get<sl::CameraComponent>(entity).m_isMainCamera = false;
				}
				pComponent->m_isMainCamera = true;
			}
			else
			{
				SL_LOG_WARN("Main camera must exist in the scene!");
			}
		}

		constexpr size_t ProjectionTypeCount = nameof::enum_count<sl::ProjectionType>();
		constexpr std::array<const char *, ProjectionTypeCount> ProjectionTypeNames =
		{
			nameof::nameof_enum(sl::ProjectionType::Perspective).data(),
			nameof::nameof_enum(sl::ProjectionType::Orthographic).data(),
		};

		StartWithText("Projection");
		if (ImGui::BeginCombo("##Projection", ProjectionTypeNames[(size_t)pComponent->m_projectionType], ImGuiComboFlags_WidthFitPreview))
		{
			for (size_t i = 0; i < ProjectionTypeCount; ++i)
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
			CheckDragWidgetState();

			StartWithText("Near Plane");
			if (ImGui::DragFloat("##NearPlane", &(pComponent->m_nearPlane), 0.001f, 0.01f, 1.0f))
			{
				pComponent->m_isDirty = true;
			}
			CheckDragWidgetState();

			StartWithText("Far Plane");
			if (ImGui::DragFloat("##FarPlane", &(pComponent->m_farPlane), 100.0f, 1.0f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}
			CheckDragWidgetState();

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
			CheckDragWidgetState();

			StartWithText("Near Clip");
			if (ImGui::DragFloat("##NearClip", &(pComponent->m_orthoNearClip), 100.0f, -100000.0f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}
			CheckDragWidgetState();

			StartWithText("Far Clip");
			if (ImGui::DragFloat("##FarClip", &(pComponent->m_orthoFarClip), 100.0f, -100000.0f, 100000.0f))
			{
				pComponent->m_isDirty = true;
			}
			CheckDragWidgetState();

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
			CheckDragWidgetState();

			StartWithText("Move Speed");
			ImGui::DragFloat("##MoveSpeed", &(pComponent->m_maxMoveSpeed), 0.001f, 0.001f, 0.5f);
			CheckDragWidgetState();

			StartWithText("Acceleration");
			ImGui::DragFloat("##Acceleration", &(pComponent->m_maxSpeedToAcceleration), 0.001f, 0.001f, 0.5f);
			CheckDragWidgetState();

			StartWithText("Shift Multiplier");
			ImGui::DragFloat("##ShiftMultiplier", &(pComponent->m_moveSpeedKeyShiftMultiplier), 0.01f, 0.1f, 10.0f);
			CheckDragWidgetState();

			StartWithText("Scroll Multiplier");
			ImGui::DragFloat("##ScrollMultiplier", &(pComponent->m_moveSpeedMouseScrollMultiplier), 0.01f, 0.1f, 10.0f);
			CheckDragWidgetState();

			ImGui::Unindent();
		}
	});

	// Draw Rendering component.
	DrawComponent<sl::RenderingComponent>("Rendering", [this](sl::RenderingComponent *pComponent)
	{
		StartWithText("Mesh");
		ImGui::Text(pComponent->m_optMeshResourceName ? pComponent->m_optMeshResourceName->c_str() : "");

		StartWithText("Material");
		ImGui::Text(pComponent->m_optMaterialResourceName ? pComponent->m_optMaterialResourceName->c_str() : "");

		// TODOD: Textures

		StartWithText("Base Shader");
		ImGui::Text(pComponent->m_optBaseShaderResourceName ? pComponent->m_optBaseShaderResourceName->c_str() : "");

		StartWithText("ID Shader");
		ImGui::Text(pComponent->m_optIDShaderResourceName ? pComponent->m_optIDShaderResourceName->c_str() : "");
	});

	// Draw Cornerstone component.
	DrawComponent<sl::CornerstoneComponent>("Cornerstone", [this](sl::CornerstoneComponent *pComponent)
	{
		StartWithText("Info");
		ImGui::TextWrapped(pComponent->m_info.c_str());
	});

	// Add component button.
	AlignNextWidget("Add Component");
	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("AddComponentPopup");
	}
	if (ImGui::BeginPopup("AddComponentPopup"))
	{
		AddComponentMenuItem<sl::CameraComponent>("Camera");
		AddComponentMenuItem<sl::CornerstoneComponent>("Cornerstone");
		AddComponentMenuItem<sl::RenderingComponent>("Rendering");
		ImGui::EndPopup();
	}

	ImGui::End();

	// TODO: ImGui `DragFloat` does not work very well with SDL relative mode, mouse will still hit the edge of window.
	// Waiting for `TeleportMousePos` to be completed in docking branch.(imgui #228)
	// if (dragWidgetActivated)
	// {
	// 	sl::Input::SetMouseWrapMode(true);
	// }
	// if (dragWidgetDeactivated)
	// {
	// 	sl::Input::SetMouseWrapMode(false);
	// }
}

void ImGuiLayer::ShowImGuizmoOrientation()
{
	SL_PROFILE;

	// Display a cube on the upper right corner.
	float Length = GetDPIFactor() * 5.0f;
	ImVec2 pos = ImVec2{
		(float)m_sceneViewportWindowPosX + (float)m_sceneViewportSizeX - Length,
		(float)m_sceneViewportWindowPosY + GetTitleBarSize() };

	auto view = sl::ECSWorld::GetMainCameraComponent().GetView();
	ImGuizmo::ViewManipulate(glm::value_ptr(view), Length, pos, ImVec2{ Length , Length }, 0);
}

void ImGuiLayer::ShowImGuizmoTransform()
{
	SL_PROFILE;

	static int s_delayFrameCout;
	if (m_imguizmoMode < 0 || !m_selectedEntity.IsValid() || m_selectedEntity == sl::ECSWorld::GetMainCameraEntity())
	{
		s_delayFrameCout = 1;
		ImGuizmo::Enable(false);
		return;
	}
	else if(s_delayFrameCout > 0)
	{
		// A little trick to avoid dragging ImGuizmo while selecting entity at the same time.
		--s_delayFrameCout;
		return;
	}

	// Disable ImGuizmo when camera is using.
	auto &camera = sl::ECSWorld::GetMainCameraComponent();
	ImGuizmo::Enable(!camera.IsUsing());
	ImGuizmo::AllowAxisFlip(false);

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetOrthographic(camera.m_projectionType == sl::ProjectionType::Orthographic ? true : false);
	ImGuizmo::SetRect((float)m_sceneViewportWindowPosX, (float)m_sceneViewportWindowPosY + GetTitleBarSize(),
		(float)m_sceneViewportSizeX, (float)m_sceneViewportSizeY);

	const glm::mat4 &view = camera.GetView();
	const glm::mat4 &projection = camera.GetProjection();

	auto &transform = m_selectedEntity.GetComponents<sl::TransformComponent>();
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
	SL_PROFILE;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PopStyleVar();
	RightClickFocus();

	m_sceneViewportWindowPosX = (uint32_t)ImGui::GetWindowPos().x;
	m_sceneViewportWindowPosY = (uint32_t)ImGui::GetWindowPos().y;

	// Send resize event.
	uint32_t crtSceneViewportSizeX = (uint32_t)ImGui::GetContentRegionAvail().x;
	uint32_t crtSceneViewportSizeY = (uint32_t)ImGui::GetContentRegionAvail().y;
	if (m_sceneViewportSizeX != crtSceneViewportSizeX || m_sceneViewportSizeY != crtSceneViewportSizeY)
	{
		m_sceneViewportSizeX = crtSceneViewportSizeX;
		m_sceneViewportSizeY = crtSceneViewportSizeY;

		sl::SceneViewportResizeEvent event{ m_sceneViewportSizeX , m_sceneViewportSizeY };
		m_eventCallback(event);
	}

	// Draw main frame buffer color attachment.
	uint32_t handle = sl::RenderCore::GetMainFramebuffer()->GetAttachmentHandle(0);
	ImGui::Image((void *)(uint64_t)handle, ImGui::GetContentRegionAvail(), ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

	// ImGuizmo
	ShowImGuizmoOrientation();
	ShowImGuizmoTransform();

	if (sl::ECSWorld::GetMainCameraComponent().IsUsing() ||
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
	if (mouseLocalPosX < 0 || mouseLocalPosY < 0 ||
		mouseLocalPosX >= m_sceneViewportSizeX || mouseLocalPosY >= m_sceneViewportSizeY)
	{
		SL_LOG_ERROR("`ImGuiLayer::MousePick` got an invalid coordinate: ({}, {})", mouseLocalPosX, mouseLocalPosY);
		return;
	}

	auto *pEntityIDFramebuffer = sl::RenderCore::GetEntityIDFramebuffer();
	int entityID = pEntityIDFramebuffer->ReadPixel(0, mouseLocalPosX, mouseLocalPosY);

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
		entityID, crtEntity.GetComponents<sl::TagComponent>().m_name.c_str(),
		mouseLocalPosX, mouseLocalPosY);

	m_selectedEntity = crtEntity;
}

bool ImGuiLayer::OnKeyPressed(sl::KeyPressEvent& event)
{
	if (sl::ECSWorld::GetMainCameraComponent().IsUsing() || ImGuizmo::IsUsing())
	{
		return false;
	}

	int key = event.GetKey();
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

bool ImGuiLayer::OnKeyRelease(sl::KeyReleaseEvent &event)
{
	if (event.GetKey() != SL_KEY_LALT)
	{
		return false;
	}

	// Mouse exit Editor mode and restore mouse frome hidden state.
	if (auto &mode = sl::ECSWorld::GetMainCameraComponent().m_controllerMode;
		mode == sl::CameraControllerMode::Editor)
	{
		mode = sl::CameraControllerMode::None;
		sl::Input::SetMouseWrapMode(false);
	}

	return false;
}

bool ImGuiLayer::OnMouseButtonPress(sl::MouseButtonPressEvent &event)
{
	if (!m_isMouseFreeInSceneView)
	{
		return false;
	}

	auto &camera = sl::ECSWorld::GetMainCameraComponent();
	if (event.GetButton() == SL_MOUSE_BUTTON_LEFT)
	{
		if (sl::Input::IsKeyPressed(SL_KEY_LALT))
		{
			camera.m_controllerMode = sl::CameraControllerMode::Editor;
			sl::Input::SetMouseWrapMode(true);
		}
		// It seems to be a bug of ImGuizmo that `IsOver` still returns true even if `ShowImGuizmoTransform` was not called.
		else if (!ImGuizmo::IsOver() || !m_selectedEntity.IsValid() || m_selectedEntity == sl::ECSWorld::GetMainCameraEntity())
		{
			MousePick();
		}
	}
	else if (event.GetButton() == SL_MOUSE_BUTTON_RIGHT)
	{
		camera.m_controllerMode = sl::CameraControllerMode::FPS;
		sl::Input::SetMouseWrapMode(true);
	}

	return false;
}

bool ImGuiLayer::OnMouseButtonRelease(sl::MouseButtonReleaseEvent &event)
{
	if (event.GetButton() != SL_MOUSE_BUTTON_RIGHT && event.GetButton() != SL_MOUSE_BUTTON_LEFT)
	{
		return false;
	}

	// Mouse exit FPS / Editor mode and restore mouse frome hidden state.
	if (auto &mode = sl::ECSWorld::GetMainCameraComponent().m_controllerMode;
		mode == sl::CameraControllerMode::FPS || mode == sl::CameraControllerMode::Editor)
	{
		mode = sl::CameraControllerMode::None;
		sl::Input::SetMouseWrapMode(false);
	}

	return false;
}

// For ImGuiLayer::m_dockSpaceFlag
static_assert(std::is_same_v<ImGuiDockNodeFlags, int>);
static_assert(ImGuiDockNodeFlags_None == 0);

// For ImGuiLayer::m_imguizmoMode
static_assert(std::is_same_v<std::underlying_type_t<ImGuizmo::OPERATION>, int>);
static_assert(ImGuizmo::OPERATION::TRANSLATE == 7);

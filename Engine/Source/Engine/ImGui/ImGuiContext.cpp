#include "ImGuiContext.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "Resource/Font.h"
#include "Window/Window.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <implot/implot.h>

namespace sl
{

void ImGuiContext::Init(void *pNativeWindow)
{
	// 1. Init imgui
	SL_ENGINE_INFO("Init ImGui");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	// 2. Set flags
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigViewportsNoTaskBarIcon = true;

	// 3. Load font
	// ImFont *pRegularFont = io.Fonts->AddFontFromFileTTF(sl::Path::FromeAsset("Font/Roboto/Roboto-Regular.ttf").c_str(), 16.0f);
	// ImFont *pBoldFont = io.Fonts->AddFontFromFileTTF(sl::Path::FromeAsset("Font/Roboto/Roboto-Bold.ttf").c_str(), 16.0f);
	// ImFont *pThinFont = io.Fonts->AddFontFromFileTTF(sl::Path::FromeAsset("Font/Roboto/Roboto-Thin.ttf").c_str(), 16.0f);
	ImFont *pRegularFont = io.Fonts->AddFontFromFileTTF(sl::Path::FromeAsset("Font/Open_Sans/static/OpenSans-Regular.ttf").c_str(), 16.0f);
	ImFont *pBoldFont = io.Fonts->AddFontFromFileTTF(sl::Path::FromeAsset("Font/Open_Sans/static/OpenSans-Bold.ttf").c_str(), 16.0f);
	ImFont *pThinFont = io.Fonts->AddFontFromFileTTF(sl::Path::FromeAsset("Font/Open_Sans/static/OpenSans-Light.ttf").c_str(), 16.0f);
	sl::Font::SetRegular(pRegularFont);
	sl::Font::SetBold(pBoldFont);
	sl::Font::SetThin(pThinFont);
	io.FontDefault = pRegularFont;

	// 4. Set style
	ImGui::StyleColorsDark();
	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	// 5. Init platform and Rendering backend
	ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(pNativeWindow), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiContext::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void ImGuiContext::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiContext::Submit()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// For docking
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

} // namespace sl

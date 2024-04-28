#include "ImGuiContext.h"

#include "Core/Log.h"
#include "Window/Window.h"

// TODO: Compile them into imgui.lib
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace sl
{

void ImGuiContext::Init(void *pNativeWindow)
{
	// 1. Init imgui
	SL_ENGINE_INFO("Init ImGui");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// 2. Set flags
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigViewportsNoTaskBarIcon = true;

	// 3. Load font

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
	ImGui::DestroyContext();
}

void ImGuiContext::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiContext::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// For docking
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

} // namespace sl

#include "ImGuiLayer.h"

#include "Core/Log.h"
#include "Window/Window.h"

// TODO: Compile them into imgui.lib
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace sl
{

ImGuiLayer::ImGuiLayer()
{
	SetName("ImGuiLayer");
}

ImGuiLayer::~ImGuiLayer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::OnAttach()
{
	// 1. Init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// 2. Set flags
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi Viewport Windows
	io.ConfigViewportsNoTaskBarIcon = true;

	// 3. Load font

	// 4. Set style
	ImGui::StyleColorsDark();
	// For docking
	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	// 5. Init platform and Rendering backend
	ImGui_ImplGlfw_InitForOpenGL(Window::GetInstance().GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGuiLayer::OnDetach()
{

}

void ImGuiLayer::OnEvent(Event &event)
{
	// TODO: block some input event here.
}

void ImGuiLayer::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::OnUpdate(float deltaTime)
{

}

void ImGuiLayer::OnRender()
{
	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);
}

void ImGuiLayer::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// For docking
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

} // namespace sl

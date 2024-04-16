#include "ImGuiLayer.h"

#include "Log/Log.h"
#include "Window/Window.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace sl
{

ImGuiLayer::ImGuiLayer(Window *pWindow) : m_pWindow(pWindow)
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
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi Viewport Windows
	// io.ConfigViewportsNoAutoMerge = true;
	// io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	SL_ENGINE_ASSERT(m_pWindow);
	ImGui_ImplGlfw_InitForOpenGL(m_pWindow->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGuiLayer::OnDetach()
{

}

void ImGuiLayer::OnEvent(Event &event)
{

}

void ImGuiLayer::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::OnUpdate()
{
	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);
}

void ImGuiLayer::OnRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ImGuiLayer::EndFrame()
{

}

} // namespace sl

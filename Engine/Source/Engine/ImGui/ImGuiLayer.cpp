#include "ImGuiLayer.h"

#include "Log/Log.h"
#include "Window/Window.h"

#include <GLFW/glfw3.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

namespace sl
{

ImGuiLayer::ImGuiLayer(Window *pWindow)
	: m_pWindow(pWindow)
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
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	SL_ENGINE_ASSERT(m_pWindow);
	ImGui_ImplGlfw_InitForOpenGL(m_pWindow->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGuiLayer::OnDetach()
{

}

void ImGuiLayer::OnUpdate()
{


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event &event)
{

}

} // namespace sl

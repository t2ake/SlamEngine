#include "WindowLayer.h"

#include "Event/MouseEvent.h"
#include "Scene/ECSWorld.h"
#include "Window/Input.h"
#include "Window/Window.h"

WindowLayer::~WindowLayer()
{
	delete m_pWindow;
}

void WindowLayer::OnAttach()
{

}

void WindowLayer::OnDetach()
{

}

void WindowLayer::OnEvent(sl::Event &event)
{

}

void WindowLayer::BeginFrame()
{

}

void WindowLayer::OnUpdate(float deltaTime)
{
	if (sl::CameraControllerMode::None == sl::ECSWorld::GetEditorCameraComponent().m_controllerMode)
	{
		return;
	}

	float newPosX = -1.0f;
	float newPosY = -1.0f;
	glm::vec2 globalMousePos = sl::Input::GetGlobalMousePos();

	if (globalMousePos.x <= 0)
	{
		newPosX = (float)m_pWindow->GetMonitorWidth() - 2.0f;
		newPosY = globalMousePos.y;
	}
	if (globalMousePos.x >= (float)m_pWindow->GetMonitorWidth() - 1.0f)
	{
		newPosX = 1.0f;
		newPosY = globalMousePos.y;
	}
	if (globalMousePos.y <= 0)
	{
		newPosX = globalMousePos.x;
		newPosY = (float)m_pWindow->GetMonitorHeight() - 2.0f;
	}
	if (globalMousePos.y >= (float)m_pWindow->GetMonitorHeight() - 1.0f)
	{
		newPosX = globalMousePos.x;
		newPosY = 1.0f;
	}

	if (newPosX > 0.0f && newPosY > 0.0f)
	{
		m_pWindow->SetGlobalCursorPos(newPosX, newPosY);
		sl::MouseButtonAcrossEvent event;
		m_eventCallback(event);
	}
}

void WindowLayer::OnRender()
{

}

void WindowLayer::EndFrame()
{
	m_pWindow->EndFrame();
}

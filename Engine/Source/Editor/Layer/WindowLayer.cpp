#include "WindowLayer.h"

#include "Event/MouseEvent.h"
#include "Scene/ECSWorld.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include <imgui.h>
#include <imguizmo/ImGuizmo.h>

WindowLayer::~WindowLayer()
{
	
}

void WindowLayer::OnAttach()
{

}

void WindowLayer::OnDetach()
{

}

void WindowLayer::OnEvent(sl::Event &event)
{
	sl::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<sl::MouseButtonPressEvent>(BIND_EVENT_CALLBACK(WindowLayer::OnMouseButtonPress));
	dispatcher.Dispatch<sl::MouseButtonReleaseEvent>(BIND_EVENT_CALLBACK(WindowLayer::OnMouseButtonRelease));
}

void WindowLayer::BeginFrame()
{

}

void WindowLayer::OnUpdate(float deltaTime)
{

}

void WindowLayer::OnRender()
{

}

void WindowLayer::EndFrame()
{
	m_pWindow->EndFrame();
}

bool WindowLayer::OnMouseButtonPress(sl::MouseButtonPressEvent &event)
{
	// TODO: Disable mouse when using ui draging widget.

	if (SL_MOUSE_BUTTON_RIGHT == event.GetButton() && sl::ECSWorld::GetEditorCameraComponent().IsUsing())
	{
		m_pWindow->CursorModeDisabled();
	}

	return false;
}

bool WindowLayer::OnMouseButtonRelease(sl::MouseButtonReleaseEvent &event)
{
	if (SL_MOUSE_BUTTON_RIGHT == event.GetButton() && !sl::ECSWorld::GetEditorCameraComponent().IsUsing())
	{
		m_pWindow->CursorModeNormal();
	}

	return false;
}

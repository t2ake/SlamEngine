#include "Editor.h"

#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "RenderCore/RenderCore.h"
#include "Window/Window.h"

#include "ImGuiLayer.h"
#include "SandboxLayer.h"

Editor::Editor(EditorInitor initor)
{
	sl::Log::Init();

	sl::RenderCore::Init(initor.m_backend);
	sl::Window::GetInstance().Init(std::move(initor.title), initor.m_width, initor.m_height);
	sl::Window::GetInstance().SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));
	sl::ImGuiContext::Init();

	sl::RenderCore::SetMainFrameBuffer(sl::FrameBuffer::Create(1280, 720));
	sl::RenderCore::SetDefaultState();

	m_layerStack.PushLayer(new SandboxLayer);
	m_layerStack.PushLayer(new ImGuiLayer);
}

Editor::~Editor()
{
	sl::ImGuiContext::Shutdown();

	m_layerStack.Shutdown();

	sl::Window::GetInstance().Shutdown();
}

void Editor::Run()
{
	while (m_isRunning)
	{
		BegineFrame();

		if (!m_isMinimized)
		{
			for (sl::Layer *pLayer : m_layerStack)
			{
				pLayer->OnUpdate(m_timer.GetDeltatIme());
			}

			Render();
		}

		EndFrame();
	}
}

void Editor::BegineFrame()
{
	m_timer.Update();
	sl::ImGuiContext::BeginFrame();

	for (sl::Layer *pLayer : m_layerStack)
	{
		pLayer->BeginFrame();
	}
}

void Editor::Render()
{
	for (sl::Layer *pLayer : m_layerStack)
	{
		pLayer->OnRender();
	}
}

void Editor::EndFrame()
{
	for (sl::Layer *pLayer : m_layerStack)
	{
		pLayer->EndFrame();
	}

	sl::ImGuiContext::EndFrame();
	sl::Window::GetInstance().EndFrame();
}

void Editor::OnEvent(sl::Event &event)
{
	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));
	dispatcher.Dispatch<sl::WindowResizeEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowResized));

	// Iterate layers from end to begin.
	for (auto it = std::make_reverse_iterator(m_layerStack.end());
		it != std::make_reverse_iterator(m_layerStack.begin());
		++it)
	{
		if (event.GetIsHandled())
		{
			break;
		}

		(*it)->OnEvent(event);
	}
}

bool Editor::OnWindowClose(sl::WindowCloseEvent &event)
{
	SL_EDITOR_TRACE("Window closed");

	m_isRunning = false;
	return true;
}

bool Editor::OnWindowResized(sl::WindowResizeEvent &event)
{
	uint32_t width = event.GetWidth();
	uint32_t height = event.GetHeight();

	if (0 == width || 0 == height)
	{
		SL_EDITOR_TRACE("Window minimized");

		m_isMinimized = true;
		return false;
	}
	m_isMinimized = false;

	return false;
}

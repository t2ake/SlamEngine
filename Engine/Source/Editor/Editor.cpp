#include "Editor.h"

#include "Core/Core.h"
#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "RenderCore/RenderCore.h"
#include "SandboxLayer.h"
#include "Window/Window.h"

Editor::Editor(EditorInitor initor)
{
	Init(std::move(initor));
}

Editor::~Editor()
{
	Shutdown();
}

void Editor::Init(EditorInitor initor)
{
	sl::Log::Init();

	sl::RenderCore::Init(initor.m_backend);
	sl::Window::GetInstance().Init(std::move(initor.title), initor.m_width, initor.m_height, true);
	sl::Window::GetInstance().SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));

	sl::RenderCore::SetDefaultState();

	m_layerStack.PushLayer(new sl::SandboxLayer);
	m_layerStack.PushLayer(new sl::ImGuiLayer);
}

void Editor::Shutdown()
{
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
	sl::RenderCore::Clear(SL_CLEAR_COLOR);
	sl::Window::GetInstance().BegineFrame();

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
		(*it)->OnEvent(event);
		if (event.GetIsHandled())
		{
			break;
		}
	}
}

bool Editor::OnWindowClose(sl::WindowCloseEvent &event)
{
	SL_EDITOR_TRACE("Window closed.");

	m_isRunning = false;
	return true;
}

bool Editor::OnWindowResized(sl::WindowResizeEvent &event)
{
	uint32_t width = event.GetWidth();
	uint32_t height = event.GetHeight();

	if (0 == width || 0 == height)
	{
		SL_EDITOR_TRACE("Window minimized.");

		m_isMinimized = true;
		return false;
	}
	m_isMinimized = false;

	sl::RenderCore::OnMainViewportResize(width, height);

	return false;
}

#include "Editor.h"

#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "Layer/LayerStack.h"
#include "RenderCore/RenderCore.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include "ImGuiLayer.h"
#include "SandboxLayer.h"

Editor *Editor::pInstance = nullptr;

Editor::Editor(EditorInitor initor)
{
	pInstance = this;

	sl::Log::Init();

	sl::RenderCore::Init(initor.m_backend);
	m_pWindow = new sl::Window(std::move(initor.title), initor.m_width, initor.m_height);
	m_pWindow->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));
	sl::ImGuiContext::Init(m_pWindow->GetNativeWindow());
	sl::Input::Init(m_pWindow->GetNativeWindow());

	sl::RenderCore::SetMainFrameBuffer(sl::FrameBuffer::Create(1280, 720));
	sl::RenderCore::SetDefaultState();

	m_pLayerStack = new sl::LayerStack;
	m_pLayerStack->PushLayer(new SandboxLayer);
	m_pLayerStack->PushLayer(new ImGuiLayer);
}

Editor::~Editor()
{
	sl::ImGuiContext::Shutdown();

	delete m_pLayerStack;
	delete m_pWindow;
}

void Editor::Run()
{
	while (m_isRunning)
	{
		BegineFrame();

		if (!m_isMinimized)
		{
			for (sl::Layer *pLayer : *m_pLayerStack)
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

	for (sl::Layer *pLayer : *m_pLayerStack)
	{
		pLayer->BeginFrame();
	}
}

void Editor::Render()
{
	for (sl::Layer *pLayer : *m_pLayerStack)
	{
		pLayer->OnRender();
	}
}

void Editor::EndFrame()
{
	for (sl::Layer *pLayer : *m_pLayerStack)
	{
		pLayer->EndFrame();
	}

	sl::ImGuiContext::EndFrame();
	m_pWindow->EndFrame();
}

void Editor::OnEvent(sl::Event &event)
{
	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));
	dispatcher.Dispatch<sl::WindowResizeEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowResized));

	// Iterate layers from end to begin.
	for (auto it = std::make_reverse_iterator(m_pLayerStack->end());
		it != std::make_reverse_iterator(m_pLayerStack->begin());
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

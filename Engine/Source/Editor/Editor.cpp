#include "Editor.h"

#include "Core.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Layer/LayerStack.h"
#include "Log/Log.h"
#include "Window/Input.h"
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

	m_pWindow = new sl::Window{ std::move(initor.title), initor.m_width, initor.m_height, true };
	m_pWindow->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));
	sl::Input::GetInstance().SetWindow(m_pWindow);

	// PENDIGN: Use pointer of every layer directly instead of layer stack.
	m_pLayerStack = new sl::LayerStack;
	m_pLayerStack->PushLayer(new sl::ImGuiLayer{ m_pWindow });
}

void Editor::Shutdown()
{
	delete m_pLayerStack;
	delete m_pWindow;
}

void Editor::Update()
{
	while (m_isRunning)
	{
		BegineFrame();

		for (sl::Layer *pLayer : *m_pLayerStack)
		{
			pLayer->OnUpdate();
		}

		Render();
		EndFrame();
	}
}

void Editor::BegineFrame()
{
	m_pWindow->BegineFrame();
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
	m_pWindow->EndFrame();
	for (sl::Layer *pLayer : *m_pLayerStack)
	{
		pLayer->EndFrame();
	}
}

void Editor::OnEvent(sl::Event &event)
{
	SL_EDITOR_TRACE(event);

	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));

	// Iterate layers from end to begin.
	for (auto it = std::make_reverse_iterator(m_pLayerStack->end()); it != std::make_reverse_iterator(m_pLayerStack->begin()); ++it)
	{
		(*it)->OnEvent(event);
		if (event.GetIsHandled())
		{
			break;
		}
	}
}

bool Editor::OnWindowClose(sl::Event &event)
{
	m_isRunning = false;
	return true;
}

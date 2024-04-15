#include "Editor.h"

#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Log/Log.h"
#include "Window/Window.h"
#include "ImGui/ImGuiLayer.h"

#define BIND_EVENT_CALLBACK(fun) std::bind(&fun, this, std::placeholders::_1)

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

	PushLayer(new sl::ImGuiLayer{ m_pWindow });
}

void Editor::Update()
{
	while (m_isRunning)
	{
		m_pWindow->Update();

		for (sl::Layer* pLayer : m_layerStack)
		{
			pLayer->OnUpdate();
		}
	}
}

void Editor::Render()
{

}

void Editor::Shutdown()
{
	for (sl::Layer *pLayer : m_layerStack)
	{
		delete pLayer;
	}
	m_layerStack.ClearLayers();

	delete m_pWindow;
}

void Editor::OnEvent(sl::Event &event)
{
	SL_EDITOR_TRACE(event);

	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));

	// Iterate layers from end to begin.
	for (auto it = std::make_reverse_iterator(m_layerStack.end()); it != std::make_reverse_iterator(m_layerStack.begin()); ++it)
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

void Editor::PushLayer(sl::Layer *pLayer)
{
	m_layerStack.PushLayer(pLayer);
	pLayer->OnAttach();
}

void Editor::PopLayer(sl::Layer *pLayer)
{
	m_layerStack.PopLayer(pLayer);
	pLayer->OnDetach();
}

#include "Editor.h"

#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Log/Log.h"
#include "Window/Window.h"

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
}

void Editor::Update()
{
	while (m_isRunning)
	{
		m_pWindow->Update();
		Render();
	}
}

void Editor::Render()
{

}

void Editor::Shutdown()
{
	delete m_pWindow;
}

void Editor::OnEvent(sl::Event &event)
{
	SL_EDITOR_TRACE(event);

	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));
}

bool Editor::OnWindowClose(sl::Event &event)
{
	m_isRunning = false;
	return true;
}

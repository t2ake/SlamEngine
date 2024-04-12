#include "Editor.h"

#include "Event/KeyEvent.h"
#include "Log/Log.h"
#include "Window/Window.h"

Editor::~Editor()
{
	Shutdown();
}

void Editor::Init()
{
	sl::Log::Init();

	m_pWindow = new sl::Window{ "Slam Engine", 1280, 720, true };
}

void Editor::Update()
{
	while(true)
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
	m_pWindow->Shutdown();
	delete m_pWindow;
}

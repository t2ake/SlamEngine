#include "Editor.h"

#include "Core.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Layer/LayerStack.h"
#include "Log/Log.h"
#include "RenderCore/RenderCore.h"
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

	sl::RenderCore::GetInstance().SetBackend(initor.m_backend);

	m_pWindow = new sl::Window{ std::move(initor.title), initor.m_width, initor.m_height, true };
	m_pWindow->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));
	sl::Input::GetInstance().SetWindow(m_pWindow);
	sl::RenderCore::GetInstance().SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// PENDIGN: Use pointer of every layer directly instead of layer stack.
	m_pLayerStack = new sl::LayerStack;
	m_pLayerStack->PushLayer(new sl::ImGuiLayer{ m_pWindow });

	// TEMPORARY
	{
		float vertices[21] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};

		uint32_t indices[3] = { 0, 1, 2 };

		std::string vsSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;
			out vec4 v_color;

			void main()
			{
				v_color = a_color;
				gl_Position = vec4(a_position, 1.0);
			}
		)";

		std::string fsSrc = R"(
			#version 330 core
			in vec4 v_color;
			out vec4 o_color;

			void main()
			{
				o_color = vec4(v_color);
			}
		)";

		sl::IndexBuffer *pIndexBuffer = sl::IndexBuffer::Create(indices, sizeof(indices));
		sl::VertexBuffer *pVertexBuffer = sl::VertexBuffer::Create(vertices, sizeof(vertices));
		pVertexBuffer->SetLayout(sl::VertexLayout
		{
			{"Position", sl::AttribType::Float, 3},
			{"Color", sl::AttribType::Float, 4},
		});

		m_pVertexArray = sl::VertexArray::Create();
		m_pVertexArray->SetVertexBuffer(pVertexBuffer);
		m_pVertexArray->SetIndexBuffer(pIndexBuffer);

		m_pShader = sl::Shader::Creat(std::move(vsSrc), std::move(fsSrc));
	}
}

void Editor::Shutdown()
{
	delete m_pVertexArray;
	delete m_pShader;

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
	sl::RenderCore::GetInstance().Clear(SL_CLEAR_COLOR);

	for (sl::Layer *pLayer : *m_pLayerStack)
	{
		pLayer->BeginFrame();
	}
}

void Editor::Render()
{
	// TEMPORARY
	sl::RenderCore::GetInstance().Submit(m_pVertexArray, m_pShader);

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
	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));

	// Iterate layers from end to begin.
	for (auto it = std::make_reverse_iterator(m_pLayerStack->end());
		it != std::make_reverse_iterator(m_pLayerStack->begin());
		++it)
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

#include "Editor.h"

#include "Core/Core.h"
#include "Core/Log.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Layer/LayerStack.h"
#include "RenderCore/RenderCore.h"
#include "Window/Input.h"
#include "Window/Window.h"

// TEMPLATE
#include "Core/Path.hpp"
#include <glm/gtc/matrix_transform.hpp>

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
	m_pWindow = new sl::Window{ std::move(initor.title), initor.m_width, initor.m_height, true };
	m_pWindow->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));

	sl::RenderCore::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	sl::RenderCore::DefaultBlend();

	// PENDING: Use pointer of every layer directly instead of layer stack.
	m_layerStack.PushLayer(new sl::ImGuiLayer{ m_pWindow });

	// TEMPORARY
	{
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		uint32_t indices[] = { 0, 1, 3, 1, 2, 3 };

		sl::VertexBuffer *pVertexBuffer = sl::VertexBuffer::Create(vertices, sizeof(vertices));
		pVertexBuffer->SetLayout(sl::VertexLayout
		{
			{"Position", sl::AttribType::Float, 3},
			{"UV", sl::AttribType::Float, 2},
		});
		sl::IndexBuffer *pIndexBuffer = sl::IndexBuffer::Create(indices, sizeof(indices));

		m_pVertexArray = sl::VertexArray::Create();
		m_pVertexArray->SetVertexBuffer(pVertexBuffer);
		m_pVertexArray->SetIndexBuffer(pIndexBuffer);

		m_pShader = sl::Shader::Creat("Test Shader",
			sl::Path::FromeAsset("Shader/vs_Test.glsl"),
			sl::Path::FromeAsset("Shader/fs_Test.glsl"));

		m_pTextureJoucho = sl::Texture2D::Create(sl::Path::FromeAsset("Texture/jc.png"));
		m_pTextureLogo = sl::Texture2D::Create(sl::Path::FromeAsset("Texture/logo.png"));

		m_camera.SetWindow(m_pWindow);
		m_camera.GetData().SetPosition(glm::vec3{ 0.0f, 0.0f, 5.0f });
		m_camera.GetData().SetRotationDegrees(glm::vec3{ 0.0f, -90.0f, 0.0f });
	}
}

void Editor::Shutdown()
{
	delete m_pVertexArray;
	delete m_pShader;

	m_layerStack.Shutdown();
	delete m_pWindow;
}

void Editor::Update()
{
	while (m_isRunning)
	{
		BegineFrame();

		for (sl::Layer *pLayer : m_layerStack)
		{
			pLayer->OnUpdate();
		}
		m_camera.Update(m_timmer.GetDeltatIme());

		Render();
		EndFrame();
	}
}

void Editor::BegineFrame()
{
	m_timmer.Update();

	sl::RenderCore::Clear(SL_CLEAR_COLOR);

	for (sl::Layer *pLayer : m_layerStack)
	{
		pLayer->BeginFrame();
	}
}

void Editor::Render()
{
	// TEMPORARY
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			glm::vec3 trans{ i, j, 0.0f };
			glm::mat4 modelMat = glm::translate(glm::mat4{ 1.0f }, trans * 1.1f);
			glm::mat4 mvp = m_camera.GetData().GetViewProjection() * modelMat;

			m_pShader->Bind();
			//m_pShader->UploadUniform("u_texture", (int)0);
			m_pShader->UploadUniform("u_ModelViewProjection", std::move(mvp));
			m_pTextureJoucho->Bind(0);
			sl::RenderCore::Submit(m_pVertexArray, m_pShader);
		}
	}

	glm::mat4 modelMat2 = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ 0.0f, 0.0f, 0.25f });
	modelMat2 = glm::scale(modelMat2, glm::vec3(4.0f, 2.0f, 0.0f));
	glm::mat4 mvp2 = m_camera.GetData().GetViewProjection() * modelMat2;
	m_pShader->Bind();
	//m_pShader->UploadUniform("u_texture", (int)0);
	m_pShader->UploadUniform("u_ModelViewProjection", std::move(mvp2));
	m_pTextureLogo->Bind(0);
	sl::RenderCore::Submit(m_pVertexArray, m_pShader);

	for (sl::Layer *pLayer : m_layerStack)
	{
		pLayer->OnRender();
	}
}

void Editor::EndFrame()
{
	m_pWindow->EndFrame();
	for (sl::Layer *pLayer : m_layerStack)
	{
		pLayer->EndFrame();
	}
}

void Editor::OnEvent(sl::Event &event)
{
	if (event.IsInCategory(SL_EVENT_CATEGORY_INPUT))
	{
		m_camera.OnEvent(event);
	}

	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));

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

bool Editor::OnWindowClose(sl::Event &event)
{
	m_isRunning = false;
	return true;
}

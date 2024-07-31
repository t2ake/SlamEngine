#include "Editor.h"

#include "Core/Log.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "LayerStack/LayerStack.h"
#include "RenderCore/RenderCore.h"
#include "Scene/ECSWorld.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include "Layer/CameraControllerLayer.h"
#include "Layer/ImGuiLayer.h"
#include "Layer/RendererLayer.h"
#include "Layer/ResourceManagerLayer.h"
#include "Layer/SandboxLayer.h"

Editor::Editor(EditorInitor initor)
{
	sl::Log::Init();
	
	sl::RenderCore::SetBackend(initor.m_backend);

	auto &window = sl::Window::GetInstance();
	window.Init(initor.title, initor.m_width, initor.m_height);
	window.SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));
	sl::Input::Init(window.GetNativeWindow());
	sl::ImGuiContext::Init(window.GetNativeWindow());

	sl::RenderCore::Init();
	sl::RenderCore::SetDefaultState();

	// Size is meaningless here.
	sl::RenderCore::SetMainFramebuffer(sl::FrameBuffer::Create(
	{
		sl::Texture2D::Create(1, 1, false, sl::TextureFormat::RGBA8, SL_SAMPLER_CLAMP | SL_SAMPLER_BILINEAR),
		sl::Texture2D::Create(1, 1, false, sl::TextureFormat::D32, SL_SAMPLER_CLAMP | SL_SAMPLER_BILINEAR),
	}));
	sl::RenderCore::SetEntityIDFramebuffer(sl::FrameBuffer::Create(
	{
		sl::Texture2D::Create(1, 1, false, sl::TextureFormat::R32I, SL_SAMPLER_CLAMP | SL_SAMPLER_NEAREST),
		sl::Texture2D::Create(1, 1, false, sl::TextureFormat::D32, SL_SAMPLER_CLAMP | SL_SAMPLER_BILINEAR),
	}));

	sl::RenderCore::SetUniformBuffer(0, sl::UniformBuffer::Create(0, sl::UniformBufferLayout
	{
		{ "u_viewProjection", sl::AttribType::mat4f },
		{ "u_cameraPos", sl::AttribType::vec4f },
	}));
		
	sl::Entity mainCameraEntity = sl::ECSWorld::CreateEntity("Editor Camera");
	mainCameraEntity.AddComponent<sl::CameraComponent>();
	mainCameraEntity.AddComponent<sl::CornerstoneComponent>("Currently we only support that only one camera in the scene.");
	sl::ECSWorld::SetEditorCameraEntity(mainCameraEntity);

	auto pRendererLayer = std::make_unique<RendererLayer>();
	auto pResourceManagerLayer = std::make_unique<ResourceManagerLayer>();
	auto pCameraControllerLayer = std::make_unique<CameraControllerLayer>();
	auto pImGuiLayer = std::make_unique<ImGuiLayer>();
	auto pSandboxLayer = std::make_unique<SandboxLayer>();

	pImGuiLayer->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));

	m_pLayerStack = std::make_unique<sl::LayerStack>();
	m_pLayerStack->PushLayer(std::move(pRendererLayer));
	m_pLayerStack->PushLayer(std::move(pResourceManagerLayer));
	m_pLayerStack->PushLayer(std::move(pCameraControllerLayer));
	m_pLayerStack->PushLayer(std::move(pImGuiLayer));
	m_pLayerStack->PushLayer(std::move(pSandboxLayer));

	m_clock.Tick();
}

Editor::~Editor()
{
	sl::ImGuiContext::Shutdown();
	sl::Window::GetInstance().Terminate();
}

void Editor::Run()
{
	while (m_isRunning)
	{
		BegineFrame();

		if (!m_isMinimized)
		{
			Update();
			Render();
		}

		EndFrame();
	}
}

void Editor::BegineFrame()
{
	m_clock.Tick();

	m_pLayerStack->BeginFrame();
}

void Editor::Update()
{
	m_pLayerStack->Update(m_clock.GetDeltatIme());
}

void Editor::Render()
{
	m_pLayerStack->Render();
}

void Editor::EndFrame()
{
	m_pLayerStack->EndFrame();

	sl::Window::GetInstance().EndFrame();
}

void Editor::OnEvent(sl::Event &event)
{
	sl::EventDispatcher dispatcher{ event };
	dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));
	dispatcher.Dispatch<sl::WindowResizeEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowResize));

	// Iterate layers from top to bottom / from end to begin.
	for (auto it = m_pLayerStack->rend(); it != m_pLayerStack->rbegin(); ++it)
	{
		if (event.IsHandled())
		{
			break;
		}
		(*it)->OnEvent(event);
	}
}

bool Editor::OnWindowClose(sl::WindowCloseEvent &event)
{
	SL_LOG_TRACE("Window close");
	m_isRunning = false;

	return true;
}

bool Editor::OnWindowResize(sl::WindowResizeEvent &event)
{
	if (event.GetWidth() == 0 || event.GetHeight() == 0)
	{
		SL_LOG_TRACE("Window minimise");
		m_isMinimized = true;
	}
	else
	{
		m_isMinimized = false;
	}

	return false;
}

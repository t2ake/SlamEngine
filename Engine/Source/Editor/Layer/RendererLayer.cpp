#include "RendererLayer.h"

#include "Event/SceneViewportEvent.h"
#include "RenderCore/RenderCore.h"
#include "Resource/ResourceManager.h"
#include "Scene/ECSWorld.h"

void RendererLayer::OnAttach()
{

}

void RendererLayer::OnDetach()
{

}

void RendererLayer::OnEvent(sl::Event &event)
{
	sl::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<sl::SceneViewportResizeEvent>(BIND_EVENT_CALLBACK(RendererLayer::OnSceneViewportResize));
}

void RendererLayer::BeginFrame()
{

}

void RendererLayer::OnUpdate(float deltaTime)
{

}

void RendererLayer::OnRender()
{
	m_pUniformBuffer->Upload("u_viewProjection", sl::ECSWorld::GetEditorCameraComponent().GetViewProjection());
	m_pUniformBuffer->Upload("u_cameraPos", glm::vec4{ sl::ECSWorld::GetEditorCameraEntity().GetComponent<sl::TransformComponent>().m_position, 1.0f });

	BasePass();
	EntityIDPass();
}

void RendererLayer::EndFrame()
{

}

void RendererLayer::BasePass()
{
	sl::RenderCore::GetMainFramebuffer()->Bind();
	sl::RenderCore::ClearColor(0.1f, 0.1f, 0.1f, 1.0);
	sl::RenderCore::ClearDepth(1.0f);

	auto group = sl::ECSWorld::GetRegistry().group<sl::RenderingComponent>(entt::get<sl::TransformComponent>);
	for (auto entity : group)
	{
		auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);
		if (!rendering.m_optMeshResourceName || !rendering.m_optTextureResourceName || !rendering.m_pShader)
		{
			continue;
		}

		rendering.m_pShader->Bind();
		rendering.m_pShader->UploadUniform("u_model", transform.GetTransform());

		if (auto *pTextureResource = sl::ResourceManager::GetTextureResource(rendering.m_optTextureResourceName.value()); pTextureResource)
		{
			if (sl::ResourceStatus::Ready == pTextureResource->GetStatus())
			{
				pTextureResource->GetTexture()->Bind(0);
			}
		}

		if (auto *pMeshResource = sl::ResourceManager::GetMeshResource(rendering.m_optMeshResourceName.value()); pMeshResource)
		{
			if (sl::ResourceStatus::Ready == pMeshResource->GetStatus())
			{
				sl::RenderCore::Submit(pMeshResource->GetVertexArray(), rendering.m_pShader);
			}
		}
	}

	sl::RenderCore::GetMainFramebuffer()->Unbind();
}

void RendererLayer::EntityIDPass()
{
	sl::RenderCore::GetEntityIDFramebuffer()->Bind();
	int entityIDClearData = -1;
	sl::RenderCore::GetEntityIDFramebuffer()->Clear(0, &entityIDClearData);
	sl::RenderCore::ClearDepth(1.0f);

	auto group = sl::ECSWorld::GetRegistry().group<sl::RenderingComponent>(entt::get<sl::TransformComponent>);
	for (auto entity : group)
	{
		auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);
		if (!rendering.m_optMeshResourceName || !rendering.m_pShader)
		{
			continue;
		}

		rendering.m_pIDShader->Bind();
		rendering.m_pShader->UploadUniform("u_model", transform.GetTransform());
		rendering.m_pIDShader->UploadUniform("u_entityID", (int)entity);

		if (auto *pMeshResource = sl::ResourceManager::GetMeshResource(rendering.m_optMeshResourceName.value()); pMeshResource)
		{
			if (sl::ResourceStatus::Ready == pMeshResource->GetStatus())
			{
				sl::RenderCore::Submit(pMeshResource->GetVertexArray(), rendering.m_pIDShader);
			}
		}
	}

	sl::RenderCore::GetEntityIDFramebuffer()->Unbind();
}

bool RendererLayer::OnSceneViewportResize(sl::SceneViewportResizeEvent &event)
{
	sl::RenderCore::GetMainFramebuffer()->Resize(event.GetWidth(), event.GetHeight());
	sl::RenderCore::GetEntityIDFramebuffer()->Resize(event.GetWidth(), event.GetHeight());

	return true;
}


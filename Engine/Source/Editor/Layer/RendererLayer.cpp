#include "RendererLayer.h"

#include "Core/Path.hpp"
#include "Event/SceneViewportEvent.h"
#include "RenderCore/RenderCore.h"
#include "Resource/ResourceManager.h"
#include "Scene/ECSWorld.h"
#include "Utils/ProfilerCPU.h"

RendererLayer::RendererLayer()
{
	auto pBaseShaderResource = std::make_unique<sl::ShaderResource>(
		sl::Path::FromeAsset("Shader/Base_vert.glsl"),
		sl::Path::FromeAsset("Shader/Base_frag.glsl")
	);
	sl::ResourceManager::AddShaderResource("BaseShader", std::move(pBaseShaderResource));

	auto pIDShaderResource = std::make_unique<sl::ShaderResource>(
		sl::Path::FromeAsset("Shader/EntityID_vert.glsl"),
		sl::Path::FromeAsset("Shader/EntityID_frag.glsl")
	);
	sl::ResourceManager::AddShaderResource("IDShader", std::move(pIDShaderResource));

}

void RendererLayer::OnAttach()
{

}

void RendererLayer::OnDetach()
{

}

void RendererLayer::OnEvent(sl::Event &event)
{
	SL_PROFILE;

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
	SL_PROFILE;

	sl::Entity mainCamera = sl::ECSWorld::GetMainCameraEntity();
	sl::RenderCore::GetUniformBuffer(0)->Upload("ub_viewProjection",
		mainCamera.GetComponents<sl::CameraComponent>().GetViewProjection());
	sl::RenderCore::GetUniformBuffer(0)->Upload("ub_cameraPos",
		glm::vec4{ mainCamera.GetComponents<sl::TransformComponent>().m_position, 1.0f });
	
	BasePass();
	EntityIDPass();
}

void RendererLayer::EndFrame()
{

}

void RendererLayer::BasePass()
{
	SL_PROFILE;

	sl::RenderCore::GetMainFramebuffer()->Bind();
	sl::RenderCore::ClearColor(0.1f, 0.1f, 0.1f, 1.0);
	sl::RenderCore::ClearDepth(1.0f);

	auto group = sl::ECSWorld::GetRegistry().group<sl::RenderingComponent>(entt::get<sl::TransformComponent>);
	for (auto entity : group)
	{
		auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);
		if (!rendering.m_optMeshResourceName || !rendering.m_optMaterialResourceName || !rendering.m_optBaseShaderResourceName)
		{
			continue;
		}

		auto *pMeshResource = sl::ResourceManager::GetMeshResource(rendering.m_optMeshResourceName.value());
		auto *pMaterialResource = sl::ResourceManager::GetMaterialResource(rendering.m_optMaterialResourceName.value());
		auto *pShaderResource = sl::ResourceManager::GetShaderResource(rendering.m_optBaseShaderResourceName.value());
		if (!pMeshResource || !pMaterialResource || !pShaderResource)
		{
			continue;
		}

		if (!pMeshResource->IsReady() || !pMaterialResource->IsReady() || !pShaderResource->IsReady())
		{
			continue;
		}

		auto *pShader = pShaderResource->GetShaderProgram();
		pShader->Bind();

		if (pMaterialResource->m_albedoPropertyGroup.m_useTexture)
		{
			auto *pTextureResource = sl::ResourceManager::GetTextureResource(pMaterialResource->m_albedoPropertyGroup.m_texture);
			if (!pTextureResource || !pTextureResource->IsReady())
			{
				continue;
			}

			pTextureResource->GetTexture()->Bind(0);
		}

		pShader->UploadUniform(0, transform.GetTransform());

		sl::RenderCore::Submit(pMeshResource->GetVertexArray(), pShader);
	}

	sl::RenderCore::GetMainFramebuffer()->Unbind();
}

void RendererLayer::EntityIDPass()
{
	SL_PROFILE;

	sl::RenderCore::GetEntityIDFramebuffer()->Bind();
	constexpr int entityIDClearData = -1;
	sl::RenderCore::GetEntityIDFramebuffer()->Clear(0, &entityIDClearData);
	sl::RenderCore::ClearDepth(1.0f);

	auto group = sl::ECSWorld::GetRegistry().group<sl::RenderingComponent>(entt::get<sl::TransformComponent>);
	for (auto entity : group)
	{
		auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);
		if (!rendering.m_optIDShaderResourceName || !rendering.m_optMeshResourceName)
		{
			continue;
		}

		auto *pShaderResource = sl::ResourceManager::GetShaderResource(rendering.m_optIDShaderResourceName.value());
		auto *pMeshResource = sl::ResourceManager::GetMeshResource(rendering.m_optMeshResourceName.value());
		if (!pShaderResource ||!pMeshResource)
		{
			continue;
		}

		if (!pShaderResource->IsReady() || !pMeshResource->IsReady())
		{
			continue;
		}

		auto *pShader = pShaderResource->GetShaderProgram();
		pShader->Bind();
		pShader->UploadUniform(0, transform.GetTransform());
		pShader->UploadUniform(1, (int)entity);

		sl::RenderCore::Submit(pMeshResource->GetVertexArray(), pShader);
	}

	sl::RenderCore::GetEntityIDFramebuffer()->Unbind();
}

bool RendererLayer::OnSceneViewportResize(sl::SceneViewportResizeEvent &event)
{
	sl::RenderCore::GetMainFramebuffer()->Resize(event.GetWidth(), event.GetHeight());
	sl::RenderCore::GetEntityIDFramebuffer()->Resize(event.GetWidth(), event.GetHeight());

	return true;
}

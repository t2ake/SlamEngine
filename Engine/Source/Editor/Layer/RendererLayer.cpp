#include "RendererLayer.h"

#include "Core/Path.hpp"
#include "Event/SceneViewportEvent.h"
#include "RenderCore/RenderCore.h"
#include "Resource/ResourceManager.h"
#include "Scene/ECSWorld.h"
#include "Utils/EnumOf.hpp"
#include "Utils/ProfilerCPU.h"

#include <glm/gtc/type_ptr.hpp>

namespace
{

void UploadMaterialPropertyGroup(sl::Shader *pShader, const auto &propertyGroup)
{
	SL_ASSERT(requires{ propertyGroup.m_useTexture; propertyGroup.m_texture; propertyGroup.m_factor; });
	SL_ASSERT(requires{ propertyGroup.m_textureSlot; propertyGroup.m_useTextureLocation; propertyGroup.m_factorLocation; });

	if (propertyGroup.m_useTexture)
	{
		auto *pTextureResource = sl::ResourceManager::GetTextureResource(propertyGroup.m_texture);
		if (pTextureResource && pTextureResource->IsReady())
		{
			pTextureResource->GetTexture()->Bind(propertyGroup.m_textureSlot);
			pShader->UploadUniform(propertyGroup.m_useTextureLocation, true);
		}
	}
	else
	{
		pShader->UploadUniform(propertyGroup.m_useTextureLocation, false);
	}

	pShader->UploadUniform(propertyGroup.m_factorLocation, propertyGroup.m_factor);
}

} // namespace

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

	// Upload camera uniform buffer.
	if (auto *pCameraUniformBuffer = sl::RenderCore::GetUniformBuffer("CameraUniformBuffer"); pCameraUniformBuffer)
	{
		sl::Entity mainCamera = sl::ECSWorld::GetMainCameraEntity();
		pCameraUniformBuffer->Upload("ub_viewProjection", glm::value_ptr(mainCamera.GetComponents<sl::CameraComponent>().GetViewProjection()));
		pCameraUniformBuffer->Upload("ub_cameraPos", glm::value_ptr(mainCamera.GetComponents<sl::TransformComponent>().m_position));
	}
	
	// Upload light uniform buffer.
	auto group = sl::ECSWorld::GetRegistry().group<sl::LightComponent>(entt::get<sl::TransformComponent>);
	uint32_t lightCount = (uint32_t)group.size();
	static std::vector<sl::LightUniformBuffer> lightGPUData(LIGHT_MAX_COUNT);
	lightGPUData.clear();
	for (auto entity : group)
	{
		auto [light, transform] = group.get<sl::LightComponent, sl::TransformComponent>(entity);

		sl::LightUniformBuffer uniformBuffer;
		uniformBuffer.type = (uint32_t)light.type;
		uniformBuffer.intensity = light.intensity;
		uniformBuffer.range = light.range;

		float cosInner = std::cos(light.inner);
		float cosOuter = std::cos(light.outer);
		float scale = 1.0f / std::max(cosInner - cosOuter, 0.001f);
		float offset =- cosOuter * scale;

		uniformBuffer.scale = scale;
		uniformBuffer.offset = offset;
		uniformBuffer.color = glm::vec4{ light.color, 0.0f };
		uniformBuffer.position = glm::vec4{ transform.m_position, 0.0f };
		uniformBuffer.direction = glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f } * transform.GetRotate();
		uniformBuffer.direction /= uniformBuffer.direction.w;
		lightGPUData.emplace_back(std::move(uniformBuffer));
	}
	if (auto *pLightUniformBuffer = sl::RenderCore::GetUniformBuffer("LightUniformBuffer"); pLightUniformBuffer)
	{
		pLightUniformBuffer->Upload("ub_lights", lightGPUData.data(), lightCount * sizeof(sl::LightUniformBuffer));
		pLightUniformBuffer->Upload("ub_lightCount", &lightCount);
	}

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
		if(!pMeshResource || !pMeshResource->IsReady() ||
			!pMaterialResource || !pMaterialResource->IsReady() ||
			!pShaderResource || !pShaderResource->IsReady())
		{
			continue;
		}

		auto *pShader = pShaderResource->GetShaderProgram();
		pShader->Bind();

		glm::mat4 modelMat = transform.GetTransform();
		pShader->UploadUniform(0, modelMat);
		pShader->UploadUniform(1, glm::transpose(glm::inverse(modelMat)));

		UploadMaterialPropertyGroup(pShader, pMaterialResource->m_albedoPropertyGroup);
		UploadMaterialPropertyGroup(pShader, pMaterialResource->m_normalPropertyGroup);
		UploadMaterialPropertyGroup(pShader, pMaterialResource->m_emissivePropertyGroup);
		UploadMaterialPropertyGroup(pShader, pMaterialResource->m_occlusionPropertyGroup);
		UploadMaterialPropertyGroup(pShader, pMaterialResource->m_roughnessPropertyGroup);
		UploadMaterialPropertyGroup(pShader, pMaterialResource->m_metallicPropertyGroup);
		
		pShader->UploadUniform(pMaterialResource->m_reflectanceLocation, pMaterialResource->m_reflectance);

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

#include "RendererLayer.h"

#include "RenderCore/RenderCore.h"
#include "Scene/ECSWorld.h"

void RendererLayer::OnAttach()
{

}

void RendererLayer::OnDetach()
{

}

void RendererLayer::OnEvent(sl::Event &event)
{

}

void RendererLayer::BeginFrame()
{

}

void RendererLayer::OnUpdate(float deltaTime)
{

}

void RendererLayer::OnRender()
{
	auto &camera = sl::ECSWorld::GetMainCameraEntity().GetComponent<sl::CameraComponent>();
	const glm::mat4 &viewProjection = camera.GetViewProjection();

	auto group = sl::ECSWorld::GetRegistry().group<sl::RenderingComponent>(entt::get<sl::TransformComponent>);
	for (auto entity : group)
	{
		sl::RenderCore::GetMainFrameBuffer()->Bind();
		sl::RenderCore::Clear(SL_CLEAR_COLOR);

		auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);

		rendering.m_pShader->Bind();
		rendering.m_pShader->UploadUniform("u_ModelViewProjection", viewProjection * transform.GetTransform());
		rendering.GetTexture(0)->Bind(0);
		sl::RenderCore::Submit(rendering.m_pVertexArray, rendering.m_pShader);
		sl::RenderCore::GetMainFrameBuffer()->Unbind();
	}
}

void RendererLayer::EndFrame()
{

}

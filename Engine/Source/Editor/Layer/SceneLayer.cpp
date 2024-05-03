#include "SceneLayer.h"

#include "Scene/ECSWorld.h"

void SceneLayer::OnAttach()
{

}

void SceneLayer::OnDetach()
{

}
void SceneLayer::OnEvent(sl::Event &event)
{
	sl::ECSWorld::GetMainCameraComponent().OnEvent(event);
}

void SceneLayer::BeginFrame()
{

}
void SceneLayer::OnUpdate(float deltaTime)
{
	sl::ECSWorld::GetMainCameraComponent().Update(deltaTime);
}

void SceneLayer::OnRender()
{

}

void SceneLayer::EndFrame()
{

}

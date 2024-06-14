#include "ResourceManagerLayer.h"

#include "Resource/ResourceManager.h"

ResourceManagerLayer::ResourceManagerLayer()
{

}

ResourceManagerLayer::~ResourceManagerLayer()
{

}

void ResourceManagerLayer::OnAttach()
{

}

void ResourceManagerLayer::OnDetach()
{

}

void ResourceManagerLayer::OnEvent(sl::Event &event)
{

}

void ResourceManagerLayer::BeginFrame()
{

}

void ResourceManagerLayer::OnUpdate(float deltaTime)
{
	sl::ResourceManager::Update();
}

void ResourceManagerLayer::OnRender()
{

}

void ResourceManagerLayer::EndFrame()
{

}

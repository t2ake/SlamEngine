#include "ResourceManager.h"

#include "Core/Core.h"
#include "Core/Log.h"
#include "Resource/TextureResource.h"

#include <nameof/nameof.hpp>

namespace sl
{

void ResourceManager::Update()
{
	for (auto &[_, pResource] : m_pMeshResources)
	{
		pResource->Update();
	}
	for (auto &[_, pResource] : m_pTextureResources)
	{
		pResource->Update();
	}
}

void ResourceManager::AddTextureResource(std::string_view name, std::unique_ptr<TextureResource> pResource)
{
	if (const auto &it = m_pTextureResources.find(name.data()); m_pTextureResources.end() != it)
	{
		SL_ENGINE_WARN("Resource \"{}\" already exists!", name.data());
		return;
	}

	m_pTextureResources[name.data()] = std::move(pResource);
}

TextureResource *ResourceManager::GetTextureResource(std::string_view name)
{
	if (const auto &it = m_pTextureResources.find(name.data()); m_pTextureResources.end() != it)
	{
		return it->second.get();
	}
	else
	{
		SL_ENGINE_WARN("Resource \"{}\" does not exist!", name.data());
		return nullptr;
	}
}

void ResourceManager::AddMeshResource(std::string_view name, std::unique_ptr<MeshResource> pResource)
{
	if (const auto &it = m_pMeshResources.find(name.data()); m_pMeshResources.end() != it)
	{
		SL_ENGINE_WARN("Resource \"{}\" already exists!", name.data());
		return;
	}

	m_pMeshResources[name.data()] = std::move(pResource);
}

MeshResource *ResourceManager::GetMeshResource(std::string_view name)
{
	if (const auto &it = m_pMeshResources.find(name.data()); m_pMeshResources.end() != it)
	{
		return it->second.get();
	}
	else
	{
		SL_ENGINE_WARN("Resource \"{}\" does not exist!", name.data());
		return nullptr;
	}
}

} // namespace sl

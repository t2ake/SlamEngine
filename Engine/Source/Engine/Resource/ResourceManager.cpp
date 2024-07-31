#include "ResourceManager.h"

#include "Core/Core.h"
#include "Core/Log.h"

#include <nameof/nameof.hpp>

namespace sl
{

void ResourceManager::Update()
{
	for (auto& [_, pResource] : m_pShaderResources)
	{
		pResource->Update();
	}
	for (auto& [_, pResource] : m_pTextureResources)
	{
		pResource->Update();
	}
	for (auto &[_, pResource] : m_pMeshResources)
	{
		pResource->Update();
	}
}

void ResourceManager::AddShaderResource(std::string_view name, std::unique_ptr<ShaderResource> pResource)
{
	AddResource(name, std::move(pResource));
}

ShaderResource* ResourceManager::GetShaderResource(std::string_view name)
{
	return GetResource<ShaderResource>(name);
}

void ResourceManager::AddTextureResource(std::string_view name, std::unique_ptr<TextureResource> pResource)
{
	AddResource(name, std::move(pResource));
}

TextureResource *ResourceManager::GetTextureResource(std::string_view name)
{
	return GetResource<TextureResource>(name);
}

void ResourceManager::AddMeshResource(std::string_view name, std::unique_ptr<MeshResource> pResource)
{
	AddResource(name, std::move(pResource));
}

MeshResource *ResourceManager::GetMeshResource(std::string_view name)
{
	return GetResource<MeshResource>(name);
}

template<class T>
static void ResourceManager::AddResource(std::string_view name, std::unique_ptr<T> pResource)
{
	if constexpr (std::is_same_v<T, ShaderResource>)
	{
		if (auto it = m_pShaderResources.find(name.data()); it == m_pShaderResources.end())
		{
			m_pShaderResources[name.data()] = std::move(pResource);
			return;
		}
	}
	else if constexpr (std::is_same_v<T, TextureResource>)
	{
		if (auto it = m_pTextureResources.find(name.data()); it == m_pTextureResources.end())
		{
			m_pTextureResources[name.data()] = std::move(pResource);
			return;
		}
	}
	else if constexpr (std::is_same_v<T, MeshResource>)
	{
		if (auto it = m_pMeshResources.find(name.data()); it == m_pMeshResources.end())
		{
			m_pMeshResources[name.data()] = std::move(pResource);
			return;
		}
	}
	else
	{
		SL_ASSERT(false, "Unknown resource type!");
	}

	SL_LOG_WARN("Resource \"{}\" already exists!", name.data());
}

template<class T>
static T* ResourceManager::GetResource(std::string_view name)
{
	if constexpr (std::is_same_v<T, ShaderResource>)
	{
		if (auto it = m_pShaderResources.find(name.data()); it != m_pShaderResources.end())
		{
			return it->second.get();
		}
	}
	else if constexpr (std::is_same_v<T, TextureResource>)
	{
		if (auto it = m_pTextureResources.find(name.data()); it != m_pTextureResources.end())
		{
			return it->second.get();
		}
	}
	else if constexpr (std::is_same_v<T, MeshResource>)
	{
		if (auto it = m_pMeshResources.find(name.data()); it != m_pMeshResources.end())
		{
			return it->second.get();
		}
	}
	else
	{
		SL_ASSERT(false, "Unknown resource type!");
	}

	SL_LOG_WARN("Resource \"{}\" does not exist!", name.data());
	return nullptr;
}

} // namespace sl

#include "ResourceManager.h"

#include "Core/Log.h"
#include "Resource/TextureResource.h"

namespace sl
{

void ResourceManager::Update()
{
	for (auto &[_, pResource] : m_resources)
	{
		pResource->Update();
	}
}

void ResourceManager::AddResource(std::string_view name, std::unique_ptr<Resource> pResource)
{
	if (const auto &it = m_resources.find(name.data()); m_resources.end() != it)
	{
		SL_ENGINE_WARN("Resource \"{}\" already exists!", name.data());
		return;
	}

	m_resources[name.data()] = std::move(pResource);
}

Resource *ResourceManager::GetResource(std::string_view name)
{
	if (const auto &it = m_resources.find(name.data()); m_resources.end() != it)
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

#include "ResourceManager.h"


namespace sl
{

void ResourceManager::Update()
{
	for (auto &[_, pResource] : m_resources)
	{
		pResource->Update();
	}
}

} // namespace sl

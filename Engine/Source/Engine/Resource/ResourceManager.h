#pragma once

#include "Resource/TextureResource.h"

#include <map>
#include <memory>
#include <string>
#include <string_view>

namespace sl
{

class Resource;

class ResourceManager final
{
public:
	ResourceManager() = delete;

	static void Update();

	static void AddResource(std::string_view name, std::unique_ptr<Resource> pResource);
	static Resource *GetResource(std::string_view name);

private:
	static inline std::map<std::string, std::unique_ptr<Resource>> m_resources;
};

} // namespace sl

#pragma once

#include "Core/Core.h"
#include "Core/Log.h"
#include "Resource/TextureResource.h"

#include <nameof/nameof.hpp>

#include <map>
#include <memory>
#include <string>
#include <string_view>

namespace sl
{

template<class T>
SL_FORCEINLINE inline std::string GetResourceName(std::string_view name)
{
	return std::string{ nameof::nameof_type<T>() } + name.data();
}

class ResourceManager final
{
public:
	ResourceManager() = delete;

	static void Update();

	template<class T>
	static void AddResource(std::string_view name, T *pTexture)
	{
		static_assert(std::is_base_of_v<Resource, T>);

		std::string key = GetResourceName<T>(name);
		const auto &it = m_resources.find(key);
		if (m_resources.end() != it)
		{
			SL_ENGINE_WARN("Resource \"{}\" already exists!", key.c_str());
			return;
		}

		m_resources[key.c_str()].reset(pTexture);
	}

	template<class T>
	static TextureResource *GetResource(std::string_view name)
	{
		static_assert(std::is_base_of_v<Resource, T>);

		std::string key = GetResourceName<T>(name);
		const auto &it = m_resources.find(key);
		return m_resources.end() == it ? nullptr : static_cast<T *>(it->second.get());
	}

private:
	static inline std::map<std::string, std::unique_ptr<Resource>> m_resources;
};

} // namespace sl

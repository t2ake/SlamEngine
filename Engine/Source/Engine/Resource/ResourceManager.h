#pragma once

#include "Resource/MeshResource.h"
#include "Resource/TextureResource.h"

#include <map>
#include <memory>
#include <string>
#include <string_view>

namespace sl
{

class ResourceManager final
{
public:
	ResourceManager() = delete;

	static void Update();

	static void AddTextureResource(std::string_view name, std::unique_ptr<TextureResource> pResource);
	static TextureResource *GetTextureResource(std::string_view name);

	static void AddMeshResource(std::string_view name, std::unique_ptr<MeshResource> pResource);
	static MeshResource *GetMeshResource(std::string_view name);

private:
	static inline std::map<std::string, std::unique_ptr<TextureResource>> m_pTextureResources;
	static inline std::map<std::string, std::unique_ptr<MeshResource>> m_pMeshResources;
};

} // namespace sl

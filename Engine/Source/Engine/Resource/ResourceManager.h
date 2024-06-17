#pragma once

#include "Resource/MeshResource.h"
#include "Resource/ShaderResource.h"
#include "Resource/TextureResource.h"

#include <map>

namespace sl
{

class ResourceManager final
{
public:
	ResourceManager() = delete;

	static void Update();

	static void AddShaderResource(std::string_view name, std::unique_ptr<ShaderResource> pResource);
	static ShaderResource* GetShaderResource(std::string_view name);

	static void AddTextureResource(std::string_view name, std::unique_ptr<TextureResource> pResource);
	static TextureResource *GetTextureResource(std::string_view name);

	static void AddMeshResource(std::string_view name, std::unique_ptr<MeshResource> pResource);
	static MeshResource *GetMeshResource(std::string_view name);

private:
	template<class T>
	static void AddResource(std::string_view name, std::unique_ptr<T> pResource);

	template<class T>
	static T *GetResource(std::string_view name);

	static inline std::map<std::string, std::unique_ptr<ShaderResource>> m_pShaderResources;
	static inline std::map<std::string, std::unique_ptr<TextureResource>> m_pTextureResources;
	static inline std::map<std::string, std::unique_ptr<MeshResource>> m_pMeshResources;
};

} // namespace sl

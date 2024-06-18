#pragma once

#include "Core/Core.h"

#include <filesystem>

namespace sl
{

class Path final
{
public:
	Path() = delete;

	static constexpr const char *RootPath{ SL_ROOT_PATH };
	static constexpr const char *AssetPath{ SL_ASSET_PATH };

	SL_FORCEINLINE static bool Exists(std::string_view path)
	{
		return std::filesystem::exists(path);
	}

	SL_FORCEINLINE static std::string NameWithExtension(std::string_view path)
	{
		return std::filesystem::path{ path }.filename().generic_string();
	}

	SL_FORCEINLINE static std::string NameWithoutExtension(std::string_view path)
	{
		return std::filesystem::path{ path }.stem().generic_string();
	}

	SL_FORCEINLINE static std::string Extension(std::string_view path)
	{
		return std::filesystem::path{ path }.extension().generic_string();
	}

	SL_FORCEINLINE static std::string FromeRoot(std::string_view path = "")
	{
		return (std::filesystem::path{ RootPath } / path).generic_string();
	}

	SL_FORCEINLINE static std::string FromeAsset(std::string_view path = "")
	{
		return (std::filesystem::path{ AssetPath } / path).generic_string();
	}

	template<class... Args>
	SL_FORCEINLINE static std::string Join(Args&&... args)
	{
		return (std::filesystem::path{ std::forward<Args>(args)} / ...).generic_string();
	}
};

} // namespace sl

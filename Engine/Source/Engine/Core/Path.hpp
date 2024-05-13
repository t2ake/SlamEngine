#pragma once

#include <filesystem>

namespace sl
{

class Path final
{
public:
	Path() = delete;

	static constexpr const char *RootPath{ SL_ROOT_PATH };
	static constexpr const char *AssetPath{ SL_ASSET_PATH };

	static bool Exists(const std::string &path)
	{
		return std::filesystem::exists(path);
	}

	static std::string FromeRoot(std::string path = "")
	{
		return (std::filesystem::path{ RootPath } / path).generic_string();
	}

	static std::string FromeAsset(std::string path = "")
	{
		return (std::filesystem::path{ SL_ASSET_PATH } / path).generic_string();
	}

	template<class... Args>
	static std::string Join(Args&&... args)
	{
		return (std::filesystem::path{ std::forward<Args>(args)} / ...).generic_string();
	}
};

} // namespace sl

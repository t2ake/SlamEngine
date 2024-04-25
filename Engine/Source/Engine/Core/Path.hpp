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

	static std::string FromeRoot(std::string path)
	{
		return (std::filesystem::path{ RootPath } / path).generic_string();
	}

	static std::string FromeAsset(std::string path)
	{
		return (std::filesystem::path{ SL_ASSET_PATH } / path).generic_string();
	}

	template<typename... Args>
	static std::string Join(std::string path, Args... args)
	{
		return (std::filesystem::path{ std::move(path) } / Join(std::move(args)...)).generic_string();
	}

private:
	static std::string Join(std::string path)
	{
		return path;
	}
};

} // namespace sl

print("Generating engine...")

project("Slam")
	kind("StaticLib")
	language("C++")
	cppdialect("C++20")
	
	-- Project, binary and intermediate file paths
	location(path.join(RootPath, "Project"))
	targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	
	defines
	{
		"SL_ROOT_PATH=\""..RootPath.."\"",
		"SL_ASSET_PATH=\""..path.join(RootPath, "Engine/Asset").."\"",
		"SPDLOG_NO_EXCEPTIONS",
		"YAML_CPP_STATIC_DEFINE",
	}
	
	-- Include paths
	includedirs
	{
		path.join(RootPath, "Engine"),
		EnginePath,
		ThirdPartyPath,
		path.join(ThirdPartyPath, "spdlog/include"),
		path.join(ThirdPartyPath, "sdl/build/include"),
		path.join(ThirdPartyPath, "sdl/build/include/SDL2"),
		path.join(ThirdPartyPath, "glad/include"),
		path.join(ThirdPartyPath, "imgui"),
		path.join(ThirdPartyPath, "glm"),
		path.join(ThirdPartyPath, "entt/src"),
		path.join(ThirdPartyPath, "yaml-cpp/include"),
		path.join(ThirdPartyPath, "shaderc/libshaderc/include"),
		path.join(ThirdPartyPath, "spirv-cross"),
		path.join(ThirdPartyPath, "tracy/public"),
		path.join(ThirdPartyPath, "assimp/include"),
		path.join(ThirdPartyPath, "assimp/build/include"),
	}
	
	-- Files
	files
	{
		path.join(EnginePath, "**.*"),
	}
	
	-- Thirdparty libs
	filter { "configurations:Debug" }
		includedirs
		{
			path.join(ThirdPartyPath, "sdl/build/include-config-debug/SDL2"),
		}
		libdirs
		{
			path.join(ThirdPartyPath, "sdl/build/Debug"),
			path.join(ThirdPartyPath, "build/glad/bin/Debug"),
			path.join(ThirdPartyPath, "build/imgui/bin/Debug"),
			path.join(ThirdPartyPath, "build/implot/bin/Debug"),
			path.join(ThirdPartyPath, "build/imguizmo/bin/Debug"),
			path.join(ThirdPartyPath, "yaml-cpp/build/Debug"),
			path.join(ThirdPartyPath, "shaderc/build/libshaderc/Debug"),
			path.join(ThirdPartyPath, "spirv-cross/build/Debug"),
			path.join(ThirdPartyPath, "tracy/build/Debug"),
			path.join(ThirdPartyPath, "assimp/build/lib/Debug"),
			path.join(ThirdPartyPath, "assimp/build/contrib/zlib/Debug"),
		}
		links
		{
			"SDL2d",
			"glad",
			"imgui",
			"implot",
			"imguizmo",
			"yaml-cppd",
			"shaderc_combined",
			"spirv-cross-cored",
			"spirv-cross-glsld",
			"TracyClient",
			"assimp-vc143-mtd",
			"zlibstaticd",
		}
	filter { "configurations:Release or configurations:Final" }
		includedirs
		{
			path.join(ThirdPartyPath, "sdl/build/include-config-release/SDL2"),
		}
		libdirs
		{
			path.join(ThirdPartyPath, "sdl/build/Release"),
			path.join(ThirdPartyPath, "build/glad/bin/Release"),
			path.join(ThirdPartyPath, "build/imgui/bin/Release"),
			path.join(ThirdPartyPath, "build/implot/bin/Release"),
			path.join(ThirdPartyPath, "build/imguizmo/bin/Release"),
			path.join(ThirdPartyPath, "yaml-cpp/build/Release"),
			path.join(ThirdPartyPath, "shaderc/build/libshaderc/Release"),
			path.join(ThirdPartyPath, "spirv-cross/build/Release"),
			path.join(ThirdPartyPath, "tracy/build/Release"),
			path.join(ThirdPartyPath, "assimp/build/lib/Release"),
			path.join(ThirdPartyPath, "assimp/build/contrib/zlib/Release"),
		}
		links
		{
			"SDL2",
			"glad",
			"imgui",
			"implot",
			"imguizmo",
			"yaml-cpp",
			"shaderc_combined",
			"spirv-cross-core",
			"spirv-cross-glsl",
			"TracyClient",
			"assimp-vc143-mt",
			"zlibstatic",
		}
	filter {}
	
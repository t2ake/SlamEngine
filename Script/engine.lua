print("Generating engine...")

project("Slam")
	kind("StaticLib")
	language("C++")
	cppdialect("C++20")
	
	-- Project, binary and intermediate file paths
	location(path.join(RootPath, "Engine/VSProject"))
	targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	
	defines
	{
		"SL_ROOT_PATH=\""..RootPath.."\"",
		"SL_ASSET_PATH=\""..path.join(RootPath, "Engine/Asset").."\"",
		"SPDLOG_NO_EXCEPTIONS", "SPDLOG_USE_STD_FORMAT",
		"YAML_CPP_STATIC_DEFINE",
	}
	
	-- Include paths
	includedirs
	{
		EnginePath,
		ThirdPartyPath,
		path.join(ThirdPartyPath, "spdlog/include"),
		path.join(ThirdPartyPath, "glfw/include"),
		path.join(ThirdPartyPath, "glad/include"),
		path.join(ThirdPartyPath, "imgui"),
		path.join(ThirdPartyPath, "glm"),
		path.join(ThirdPartyPath, "entt/src"),
		path.join(ThirdPartyPath, "yaml-cpp/include"),
		path.join(ThirdPartyPath, "shaderc/libshaderc/include"),
		path.join(ThirdPartyPath, "spirv-cross"),
	}
	
	-- Files
	files
	{
		path.join(EnginePath, "**.*"),
	}
	
	-- Thirdparty libs
	filter { "configurations:Debug" }
		libdirs
		{
			path.join(ThirdPartyPath, "glfw/build/src/Debug"),
			path.join(ThirdPartyPath, "build/glad/bin/Debug"),
			path.join(ThirdPartyPath, "build/imgui/bin/Debug"),
			path.join(ThirdPartyPath, "build/implot/bin/Debug"),
			path.join(ThirdPartyPath, "build/imguizmo/bin/Debug"),
			path.join(ThirdPartyPath, "yaml-cpp/build/Debug"),
			path.join(ThirdPartyPath, "shaderc/build/libshaderc/Debug"),
			path.join(ThirdPartyPath, "spirv-cross/build/Debug"),
		}
		links
		{
			"glfw3", "glad", "imgui", "implot", "imguizmo", "yaml-cppd", "shaderc_combined", "spirv-cross-cored", "spirv-cross-glsld",
		}
	filter { "configurations:Release or configurations:Final" }
		libdirs
		{
			path.join(ThirdPartyPath, "glfw/build/src/Release"),
			path.join(ThirdPartyPath, "build/glad/bin/Release"),
			path.join(ThirdPartyPath, "build/imgui/bin/Release"),
			path.join(ThirdPartyPath, "build/implot/bin/Release"),
			path.join(ThirdPartyPath, "build/imguizmo/bin/Release"),
			path.join(ThirdPartyPath, "yaml-cpp/build/Release"),
			path.join(ThirdPartyPath, "shaderc/build/libshaderc/Release"),
			path.join(ThirdPartyPath, "spirv-cross/build/Release"),
		}
		links
		{
			"glfw3", "glad", "imgui", "implot", "imguizmo", "yaml-cpp", "shaderc_combined", "spirv-cross-core", "spirv-cross-glsl",
		}
	filter {}
	
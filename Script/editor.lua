print("Generating editor...")

project("Editor")
	kind("ConsoleApp")
	language("C++")
	cppdialect("C++20")
	dependson { "Slam" }
	
	-- Project, binary and intermediate file paths
	location(path.join(RootPath, "Engine/VSProject"))
	targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	
	-- Definitions
	filter { "configurations:Debug" }
		defines { "SL_DEBUG" }
	filter { "configurations:Release" }
		defines { "SL_RELEASE" }
	filter { "configurations:Final" }
		defines { "SL_FINAL" }
	filter { "system:Windows" }
		defines { "SL_WINDOWS" }
	filter {}
	
	defines
	{
		"SL_ROOT_PATH=\""..RootPath.."\"",
		"SL_ASSET_PATH=\""..path.join(RootPath, "Engine/Asset").."\"",
		"SPDLOG_NO_EXCEPTIONS", "SPDLOG_USE_STD_FORMAT",
	}
	
	-- Include paths
	includedirs
	{
		EnginePath,
		EditorPath,
		ThirdPartyPath,
		path.join(ThirdPartyPath, "spdlog/include"),
		path.join(ThirdPartyPath, "imgui"),
		path.join(ThirdPartyPath, "glm"),
		path.join(ThirdPartyPath, "entt/src"),
	}
	
	-- Files
	files
	{
		path.join(EditorPath, "**.*"),
	}
	
	-- Thirdparty libs
	filter { "configurations:Debug" }
		libdirs
		{
			path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Slam"),
			path.join(ThirdPartyPath, "build/imgui/bin/Debug"),
			path.join(ThirdPartyPath, "build/imguizmo/bin/Debug"),
		}
		links
		{
			"Slam", "imgui", "imguizmo",
		}
	filter { "configurations:Release or configurations:Final" }
		libdirs
		{
			path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Slam"),
			path.join(ThirdPartyPath, "build/imgui/bin/Release"),
			path.join(ThirdPartyPath, "build/imguizmo/bin/Release"),
		}
		links
		{
			"Slam", "imgui", "imguizmo",
		}
	filter {}
	
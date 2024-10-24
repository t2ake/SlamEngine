print("Generating editor...")

project("Editor")
	kind("ConsoleApp")
	language("C++")
	cppdialect("C++20")
	dependson { "Slam" }
	
	-- Project, binary and intermediate file paths
	location(path.join(RootPath, "Project"))
	targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	
	defines
	{
		"SL_ROOT_PATH=\""..RootPath.."\"",
		"SL_ASSET_PATH=\""..path.join(RootPath, "Engine/Asset").."\"",
		"SPDLOG_NO_EXCEPTIONS", "SPDLOG_USE_STD_FORMAT",
	}
	
	-- Include paths
	includedirs
	{
		path.join(RootPath, "Engine"),
		EnginePath,
		EditorPath,
		ThirdPartyPath,
		path.join(ThirdPartyPath, "spdlog/include"),
		path.join(ThirdPartyPath, "imgui"),
		path.join(ThirdPartyPath, "glm"),
		path.join(ThirdPartyPath, "entt/src"),
		path.join(ThirdPartyPath, "tracy/public"),
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
		}
		links
		{
			"Slam",
		}
		postbuildcommands 
		{
			"{COPYFILE} %[" .. path.join(ThirdPartyPath, "sdl/build/Debug/SDL2d.dll") .. "] %[%{cfg.targetdir}]",
		}
	filter { "configurations:Release or configurations:Final" }
		libdirs
		{
			path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Slam"),
		}
		links
		{
			"Slam",
		}
		postbuildcommands 
		{
			"{COPYFILE} %[" .. path.join(ThirdPartyPath, "sdl/build/Release/SDL2.dll") .. "] %[%{cfg.targetdir}]",
		}
	filter {}
	
print("Generating engine...")

project("Slam")
	kind("StaticLib")
	language("C++")
	cppdialect("C++20")
	
	-- Project, binary and intermediate files path.
	location(IntermediatePath)
	targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	
	-- Set definitions.
	defines
	{
		-- To avoid warning C4996 from fmt in spdlog.
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
		"SPDLOG_NO_EXCEPTIONS",
	}
	
	filter "configurations:Debug"
		defines { "SL_DEBUG" }
	filter "configurations:Release"
		defines { "SL_RELEASE" }
	filter "configurations:Final"
		defines { "SL_FINAL" }
	filter "system:Windows"
		defines { "SL_WINDOWS" }
	filter {}
	
	-- Set include paths.
	includedirs
	{
		EnginePath,
		ThirdPartyPath,
		
		path.join(ThirdPartyPath, "spdlog/include"),
	}
	
	-- Set files.
	files
	{
		path.join(EnginePath, "**.*"),
		path.join(ThirdPartyPath, "spdlog/include/**.*"),
	}
	
	-- Link to thirdparty libs.
	--filter { "configurations:Debug" }
	--	libdirs {
	--		path.join(ThirdPartyPath, "opencv/build/lib/Debug"),
	--	}
	--	links {
	--		"opencv_world481d",
	--	}
	--filter { "configurations:Release" }
	--	libdirs {
	--		path.join(ThirdPartyPath, "opencv/build/lib/Release"),
	--	}
	--	links {
	--		"opencv_world481",
	--	}
	--filter {}
	
	-- Use /MT and /MTd.
	staticruntime "on"
	filter "configurations:Debug"
		runtime("Debug") -- /MTd
	filter "configurations:Release"
		runtime("Release") -- /MT
	filter "configurations:Final"
		runtime("Release") -- /MT
	filter {}
	
	-- Some optimize.
	justmycode("Off")
	editAndContinue("Off")
	exceptionhandling("Off")
	rtti("Off")
	
	-- Strict.
	warnings("Default")
	externalwarnings("Off")
	
	flags
	{
		-- Compiler uses multiple thread.
		"MultiProcessorCompile",
		-- treat warnings as errors
		"FatalWarnings",
	}
	
print("")

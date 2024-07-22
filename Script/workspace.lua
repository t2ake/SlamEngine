workspace("SlamEngine")
	location(RootPath)
	
	architecture("x64")
	configurations{ "Debug", "Release", "Final" }
	staticruntime "on" -- Use static C runtime library

	-- Debug mode, no optimization
	filter { "configurations:Debug" }
		defines { "SL_DEBUG" }
		symbols("On")
		optimize("Off")
		runtime("Debug") -- /MTd
		
	-- Release mode
	filter { "configurations:Release" }
		defines { "SL_RELEASE" }
		symbols("On")
		optimize("On")
		runtime("Release") -- /MT

	-- Final maode, full optimization
	filter { "configurations:Final" }
		defines { "SL_FINAL" }
		symbols("Off")
		optimize("Full")
		runtime("Release") -- /MT

	filter { "system:Windows" }
		defines { "SL_WINDOWS" }
		systemversion("latest")
		
	filter {}
	rtti("Off")
	exceptionhandling("Off")
	justmycode("Off")

	flags
	{
		-- Treat all warnings as errors.
		"FatalWarnings",
		-- Enable Visual Studio to use multiple compiler processes when building.
		"MultiProcessorCompile",
	}

	filter { "action:vs*" }
		-- To enable __VA_OPT__ stuff for VS with C++20.
		-- https://learn.microsoft.com/en-us/cpp/preprocessor/preprocessor-experimental-overview?view=msvc-170

		-- Don't know why 'usestandardpreprocessor' doesn't works here.
		-- https://premake.github.io/docs/usestandardpreprocessor

		buildoptions { "/Zc:preprocessor" }
	filter {}

	startproject("Editor")
	
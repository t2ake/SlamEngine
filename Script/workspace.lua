workspace("SlamEngine")
	location(RootPath)
	
	architecture("x64")
	configurations{ "Debug", "Release", "Final" }
	staticruntime "on" -- Runtime library

	-- Debug mode, no optimization
	filter { "configurations:Debug" }
		symbols("On")
		optimize("Off")
		runtime("Debug") -- /MTd
		
	-- Release mode
	filter { "configurations:Release" }
		symbols("On")
		optimize("On")
		runtime("Release") -- /MT

	-- Final maode, full optimization
	filter { "configurations:Final" }
		symbols("Off")
		optimize("Full")
		runtime("Release") -- /MT

	filter { "system:Windows" }
		systemversion("latest")
		
	filter {}
	rtti("Off")
	exceptionhandling("Off")
	justmycode("Off")

	filter { "action:vs*" }
		-- To enable __VA_OPT__ stuff for VS with C++20.
		-- https://learn.microsoft.com/en-us/cpp/preprocessor/preprocessor-experimental-overview?view=msvc-170

		-- Don't know why 'usestandardpreprocessor' doesn't works here.
		-- https://premake.github.io/docs/usestandardpreprocessor

		buildoptions { "/Zc:preprocessor" }
	filter {}

	startproject("Editor")
	
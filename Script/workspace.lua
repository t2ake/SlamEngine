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
	editAndContinue("Off")

	startproject("Editor")
	
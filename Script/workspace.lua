workspace("SlamEngine")
	location(RootPath)
	
	architecture("x64")
	configurations{ "Debug", "Release", "Final" }
	
	-- No optimization in Debug mode
	filter { "configurations:Debug" }
		symbols("On")
		optimize("Off")
		
	filter { "configurations:Release" }
		symbols("On")
		optimize("On")
		
	-- Full optimization in Final maode
	filter { "configurations:Final" }
		symbols("Off")
		optimize("Full")
		
	filter { "system:Windows" }
		systemversion("latest")
		
	filter {}
	
	startproject("Editor")
	
CurrentWorkingDirectory = os.getcwd()
RootPath = string.sub(CurrentWorkingDirectory, 0, string.len(CurrentWorkingDirectory) - string.len("Script"))
ThirdPartyPath = path.join(RootPath, "Engine/Source/ThirdParty")
ThirdPartyBuildPath = path.join(ThirdPartyPath, "build")

workspace("SlamThirdparty")
	location(ThirdPartyBuildPath)
	
	architecture("x64")
	configurations{ "Debug", "Release"}
	
	-- No optimization in Debug mode.
	filter { "configurations:Debug" }
		symbols("On")
		optimize("Off")
		
	-- Full optimization in Release maode.
	filter { "configurations:Release" }
		symbols("Off")
		optimize("Full")
		
	filter { "system:Windows" }
		systemversion("latest")
		
	filter {}
	
project("glad")
	kind("StaticLib")
	language("C++")
	cppdialect("C++20")
	location(path.join(ThirdPartyBuildPath, "glad"))
	
	includedirs
	{
		path.join(ThirdPartyPath, "glad/include"),
	}
	
	files
	{
		path.join(ThirdPartyPath, "glad/**.*"),
	}
	
	-- Use /MT and /MTd.
	staticruntime "on"
	filter { "configurations:Debug" }
		runtime("Debug") -- /MTd
	filter { "configurations:Release" }
		runtime("Release") -- /MT
	filter {}
	
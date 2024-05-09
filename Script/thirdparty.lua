CurrentWorkingDirectory = os.getcwd()
RootPath = string.sub(CurrentWorkingDirectory, 0, string.len(CurrentWorkingDirectory) - string.len("Script"))
ThirdPartyPath = path.join(RootPath, "Engine/Source/ThirdParty")
ThirdPartyBuildPath = path.join(ThirdPartyPath, "build")

workspace("SlamThirdparty")
	location(ThirdPartyBuildPath)
	
	architecture("x64")
	configurations{ "Debug", "Release"}
	staticruntime "on"
	
	-- No optimization in Debug mode.
	filter { "configurations:Debug" }
		symbols("On")
		optimize("Off")
		runtime("Debug") -- /MTd
		
	-- Full optimization in Release maode.
	filter { "configurations:Release" }
		symbols("Off")
		optimize("Full")
		runtime("Release") -- /MT
		
	filter { "system:Windows" }
		systemversion("latest")
		
	filter {}
	
GladPath = path.join(ThirdPartyPath, "glad")
print("[ glad ] path: "..GladPath)
project("glad")
	kind("StaticLib")
	language("C++")
	cppdialect("C++20")
	location(path.join(ThirdPartyBuildPath, "glad"))
	
	includedirs
	{
		path.join(GladPath, "include"),
	}
	
	files
	{
		path.join(GladPath, "**.*"),
	}
	
ImGuiPath = path.join(ThirdPartyPath, "imgui")
print("[ imgui ] path: "..ImGuiPath)
project("imgui")
	kind("StaticLib")
	language("C++")
	cppdialect("C++20")
	location(path.join(ThirdPartyBuildPath, "imgui"))
	
	includedirs
	{
		ImGuiPath,
	}
	
	files
	{
		path.join(ImGuiPath, "*.h"),
		path.join(ImGuiPath, "*.cpp"),
	}
	
ImPlotPath = path.join(ThirdPartyPath, "implot")
print("[ implot ] path: "..ImPlotPath)
project("implot")
	kind("StaticLib")
	language("C++")
	cppdialect("C++20")
	location(path.join(ThirdPartyBuildPath, "implot"))
	
	includedirs
	{
		ImPlotPath,
		ImGuiPath,
	}
	
	files
	{
		path.join(ImPlotPath, "*.h"),
		path.join(ImPlotPath, "*.cpp"),
	}
	
ImGuizmoPath = path.join(ThirdPartyPath, "imguizmo")
print("[ imguizmo ] path: "..ImGuizmoPath)
project("imguizmo")
	kind("StaticLib")
	language("C++")
	cppdialect("C++20")
	location(path.join(ThirdPartyBuildPath, "imguizmo"))
	
	includedirs
	{
		ImGuizmoPath,
		ImGuiPath,
	}
	
	files
	{
		path.join(ImGuizmoPath, "*.h"),
		path.join(ImGuizmoPath, "*.cpp"),
	}
	
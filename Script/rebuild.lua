print("Generating utility...")

project("Rebuild")
	kind("Utility")
	
	-- Project, binary and intermediate files path.
	location(IntermediatePath)
	targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	
	-- Set files.
	files
	{
		path.join(RootPath, "Setup.bat"),
		path.join(RootPath, "Script/**.lua"),
	}
	
	postbuildcommands
	{
		path.join(RootPath, "Setup.bat"),
	}
	
	flags
	{
		-- Exclude a source code file from the build, for the current configuration.
		"ExcludeFromBuild",
	}
	
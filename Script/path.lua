CurrentWorkingDirectory = os.getcwd()
RootPath = string.sub(CurrentWorkingDirectory, 0, string.len(CurrentWorkingDirectory) - string.len("Script") - 1)

SourcePath = path.join(RootPath, "Engine/Source")
BinaryPath = path.join(RootPath, "Engine/Binary/bin")
IntermediatePath = path.join(RootPath, "Engine/Binary/obj")

EnginePath = path.join(SourcePath, "Engine")
EditorPath = path.join(SourcePath, "Editor")
ThirdPartyPath = path.join(SourcePath, "ThirdParty")

print("Root path: "..RootPath)

print("Source path: "..SourcePath)
print("Binary path: "..BinaryPath)
print("Intermediate path: "..IntermediatePath)

print("Engine path: "..EnginePath)
print("Editor path: "..EditorPath)
print("Thirdparty path: "..ThirdPartyPath)

print("")

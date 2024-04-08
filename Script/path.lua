CurrentWorkingDirectory = os.getcwd()
RootPath = string.sub(CurrentWorkingDirectory, 0, string.len(CurrentWorkingDirectory) - string.len("Script"))

SourcePath = path.join(RootPath, "Engine/Source")
BinaryPath = path.join(RootPath, "Engine/bin")
IntermediatePath = path.join(RootPath, "Engine/int")

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

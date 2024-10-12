# SlamEngine
[![MSBuild](https://github.com/Hinageshi01/SlamEngine/actions/workflows/msbuild.yml/badge.svg?branch=main)](https://github.com/Hinageshi01/SlamEngine/actions/workflows/msbuild.yml)
![Preview](https://github.com/roeas/SlamEngine/blob/main/Engine/Asset/Texture/Preview.png)

# Build
- Install CMake(3.15 at least)
- Install Visual Studio 2022
- Run `SetupThirdParty.bat`
- Run `Setup.bat`

# Config diff
||Debug|Release|Final|
|:-|:-:|:-:|:-:|
|Optimization|Off|On|On|
|Symbol|On|On|Off|
|Defination|`SL_DEBUG`|`SL_RELEASE`|`SL_FINAL`|
|Log|On|On|Off|
|Error check|On|On|Part|
|Assertion|On|On|Off|
|Force inline|Off|On|On|
|Shader optimization|Off|Off|On|
|Shader debug info|On|On|Off|

# Third party
[premake-core](https://github.com/premake/premake-core) v5.0.0-beta2

[spdlog](https://github.com/gabime/spdlog) v1.13.0

[SDL](https://github.com/libsdl-org/SDL) v2.30.7

[glad](https://github.com/Dav1dde/glad) v1

[imgui](https://github.com/ocornut/imgui) v1.90.5-docking

[glm](https://github.com/g-truc/glm) v1.0.1

[stb](https://github.com/nothings/stb) ae721c50eaf761660b4f90cc590453cdb0c2acd0

[entt](https://github.com/skypjack/entt) v3.13.2

[nameof](https://github.com/Neargye/nameof) v0.10.4

[implot](https://github.com/epezent/implot) v0.16

[ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) v1.84 WIP

[yaml-cpp](https://github.com/jbeder/yaml-cpp) v0.8.0

[shaderc](https://github.com/google/shaderc) v2024.2

[spirv-cross](https://github.com/KhronosGroup/SPIRV-Cross) vulkan-sdk-1.3.283.0

[tracy](https://github.com/wolfpld/tracy) v0.11.1

[IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders) bf610efb2e5d4361df799d0dfb5ae3977d7bba2e

[material-design-icons](https://github.com/google/material-design-icons) e9da2194e65080a829d670ae39a99c7b5fc09548

[Google Fonts](https://fonts.google.com/icons)

[assimp](https://github.com/assimp/assimp/) v5.4.3

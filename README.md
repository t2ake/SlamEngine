# SlamEngine
[![MSBuild](https://github.com/Hinageshi01/SlamEngine/actions/workflows/msbuild.yml/badge.svg?branch=main)](https://github.com/Hinageshi01/SlamEngine/actions/workflows/msbuild.yml)

# Build
- Install Cmake(3.15 at least)
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
|Assertion|On|On|Off|
|Error check|On|On|Part|
|Force inline|Off|Off|On|
|Shader optimization|Off|Off|On|
|Shader debug info|On|On|Off|

# Third party
premake5 v5.0.0-beta2

spdlog v1.13.0

glfw v3.4

glad v1

imgui v1.90.5-docking

glm v1.0.1

stb ae721c50eaf761660b4f90cc590453cdb0c2acd0

entt v3.13.2

nameof v0.10.4

implot v0.16

imguizmo v1.84 WIP

yaml-cpp v0.8.0

shaderc v2024.2

spirv-cross vulkan-sdk-1.3.283.0

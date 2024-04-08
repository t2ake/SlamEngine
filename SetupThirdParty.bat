@echo off

rem Set path
set "ROOT_PATH=%~dp0"
set "THIRD_PARTY_PATH=%ROOT_PATH%\Engine\Source\ThirdParty"
echo Root path: %ROOT_PATH%
echo ThirdParty path: %THIRD_PARTY_PATH%

rem Find MSBuild by vswhere
set "VSWHERE_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq tokens=*" %%i in (`"%VSWHERE_PATH%" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
  set "MSBUILD_PATH=%%i"
)
echo Found MSBuild at: %MSBUILD_PATH%

pause

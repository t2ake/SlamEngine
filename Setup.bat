@echo off

set "ROOT_PATH=%~dp0"
set "SCRIPT_PATH=%ROOT_PATH%\Script"
echo Script path: %SCRIPT_PATH%
echo.

cd %SCRIPT_PATH%
"Premake\premake5.exe" vs2022

pause

@echo off
call .\Tools\Premake\premake5.exe vs2019 --file="premake5.lua"
call .\Tools\Premake\premake5.exe smart_arguments --file="premake5.lua"
popd
PAUSE

@echo off
call .\3rdParty\premake5.exe vs2019 --file="premake5.lua"
call .\3rdParty\premake5.exe smart_arguments --file="premake5.lua"
popd
PAUSE

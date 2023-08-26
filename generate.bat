@echo off
call .\3rdParty\premake5.exe vs2019 --file="premake5.lua"
popd
PAUSE

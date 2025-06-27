@echo off

call del .\Steam.zip
call mkdir .\Steam\

call xcopy /s ..\Solution\_Build\Game\Release_x64\ .\Steam\
call xcopy /s ..\Assets\ .\Steam\Assets\
call xcopy /s ..\Levels\ .\Steam\Levels\
call ren .\Steam\Game.exe WithMyLittleEye.exe

call del .\Steam\asserts.txt
call del .\Steam\crashes.txt
call del .\Steam\debug.txt
call del .\Steam\imgui.ini
call del .\Steam\steam_appid.txt

call "C:\Program Files\7-Zip\7z.exe" a -o{..\} -sdel Steam.zip .\Steam\*
call rmdir /s /q .\Steam\

popd
pause
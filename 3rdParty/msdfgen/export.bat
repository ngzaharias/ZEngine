@echo off

cd %~dp0
echo %cd%

for %%a in (%*) do (
	echo %%~dpa

	msdfgen.exe msdf -font %%a '0' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/0.png -autoframe
	msdfgen.exe msdf -font %%a '1' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/1.png -autoframe
	msdfgen.exe msdf -font %%a '2' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/2.png -autoframe
	msdfgen.exe msdf -font %%a '3' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/3.png -autoframe
	msdfgen.exe msdf -font %%a '4' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/4.png -autoframe
	msdfgen.exe msdf -font %%a '5' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/5.png -autoframe
	msdfgen.exe msdf -font %%a '6' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/6.png -autoframe
	msdfgen.exe msdf -font %%a '7' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/7.png -autoframe
	msdfgen.exe msdf -font %%a '8' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/8.png -autoframe
	msdfgen.exe msdf -font %%a '9' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/9.png -autoframe
	msdfgen.exe msdf -font %%a 'A' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/A.png -autoframe
	msdfgen.exe msdf -font %%a 'B' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/B.png -autoframe
	msdfgen.exe msdf -font %%a 'C' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/C.png -autoframe
	msdfgen.exe msdf -font %%a 'D' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/D.png -autoframe
	msdfgen.exe msdf -font %%a 'E' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/E.png -autoframe
	msdfgen.exe msdf -font %%a 'F' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/F.png -autoframe
	msdfgen.exe msdf -font %%a 'G' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/G.png -autoframe
	msdfgen.exe msdf -font %%a 'H' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/H.png -autoframe
	msdfgen.exe msdf -font %%a 'I' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/I.png -autoframe
	msdfgen.exe msdf -font %%a 'J' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/J.png -autoframe
	msdfgen.exe msdf -font %%a 'K' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/K.png -autoframe
	msdfgen.exe msdf -font %%a 'L' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/L.png -autoframe
	msdfgen.exe msdf -font %%a 'M' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/M.png -autoframe
	msdfgen.exe msdf -font %%a 'N' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/N.png -autoframe
	msdfgen.exe msdf -font %%a 'O' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/O.png -autoframe
	msdfgen.exe msdf -font %%a 'P' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/P.png -autoframe
	msdfgen.exe msdf -font %%a 'Q' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Q.png -autoframe
	msdfgen.exe msdf -font %%a 'R' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/R.png -autoframe
	msdfgen.exe msdf -font %%a 'S' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/S.png -autoframe
	msdfgen.exe msdf -font %%a 'T' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/T.png -autoframe
	msdfgen.exe msdf -font %%a 'U' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/U.png -autoframe
	msdfgen.exe msdf -font %%a 'V' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/V.png -autoframe
	msdfgen.exe msdf -font %%a 'W' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/W.png -autoframe
	msdfgen.exe msdf -font %%a 'X' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/X.png -autoframe
	msdfgen.exe msdf -font %%a 'Y' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Y.png -autoframe
	msdfgen.exe msdf -font %%a 'Z' -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Z.png -autoframe
)

pause


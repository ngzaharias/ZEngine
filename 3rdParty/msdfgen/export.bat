@echo off

cd %~dp0
echo %cd%

for %%a in (%*) do (
	echo %%~dpa


	msdfgen.exe msdf -font %%a 0x21 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Exclamation.png -autoframe
	msdfgen.exe msdf -font %%a 0x22 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/DoubleQuote.png -autoframe
	msdfgen.exe msdf -font %%a 0x27 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/SingleQuote.png -autoframe
	msdfgen.exe msdf -font %%a 0x28 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/ParenthesesL.png -autoframe
	msdfgen.exe msdf -font %%a 0x29 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/ParenthesesR.png -autoframe
	msdfgen.exe msdf -font %%a 0x2A -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Astrix.png -autoframe
	msdfgen.exe msdf -font %%a 0x2B -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Plus.png -autoframe
	msdfgen.exe msdf -font %%a 0x2C -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Comma.png -autoframe
	msdfgen.exe msdf -font %%a 0x2D -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Minus.png -autoframe
	msdfgen.exe msdf -font %%a 0x2E -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Period.png -autoframe
	msdfgen.exe msdf -font %%a 0x2F -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/SlashF.png -autoframe
	msdfgen.exe msdf -font %%a 0x3A -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Colon.png -autoframe
	msdfgen.exe msdf -font %%a 0x3B -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Semicolon.png -autoframe
	msdfgen.exe msdf -font %%a 0x3D -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Equals.png -autoframe
	msdfgen.exe msdf -font %%a 0x3F -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Question.png -autoframe
	msdfgen.exe msdf -font %%a 0x30 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Number_0.png -autoframe
	msdfgen.exe msdf -font %%a 0x31 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Number_1.png -autoframe
	msdfgen.exe msdf -font %%a 0x32 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Number_2.png -autoframe
	msdfgen.exe msdf -font %%a 0x33 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Number_3.png -autoframe
	msdfgen.exe msdf -font %%a 0x34 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Number_4.png -autoframe
	msdfgen.exe msdf -font %%a 0x35 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Number_5.png -autoframe
	msdfgen.exe msdf -font %%a 0x36 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Number_6.png -autoframe
	msdfgen.exe msdf -font %%a 0x37 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Number_7.png -autoframe
	msdfgen.exe msdf -font %%a 0x38 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Number_8.png -autoframe
	msdfgen.exe msdf -font %%a 0x39 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Number_9.png -autoframe
	msdfgen.exe msdf -font %%a 0x41 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_A.png -autoframe
	msdfgen.exe msdf -font %%a 0x42 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_B.png -autoframe
	msdfgen.exe msdf -font %%a 0x43 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_C.png -autoframe
	msdfgen.exe msdf -font %%a 0x44 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_D.png -autoframe
	msdfgen.exe msdf -font %%a 0x45 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_E.png -autoframe
	msdfgen.exe msdf -font %%a 0x46 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_F.png -autoframe
	msdfgen.exe msdf -font %%a 0x47 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_G.png -autoframe
	msdfgen.exe msdf -font %%a 0x48 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_H.png -autoframe
	msdfgen.exe msdf -font %%a 0x49 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_I.png -autoframe
	msdfgen.exe msdf -font %%a 0x4A -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_J.png -autoframe
	msdfgen.exe msdf -font %%a 0x4B -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_K.png -autoframe
	msdfgen.exe msdf -font %%a 0x4C -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_L.png -autoframe
	msdfgen.exe msdf -font %%a 0x4D -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_M.png -autoframe
	msdfgen.exe msdf -font %%a 0x4E -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_N.png -autoframe
	msdfgen.exe msdf -font %%a 0x4F -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_O.png -autoframe
	msdfgen.exe msdf -font %%a 0x50 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_P.png -autoframe
	msdfgen.exe msdf -font %%a 0x51 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_Q.png -autoframe
	msdfgen.exe msdf -font %%a 0x52 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_R.png -autoframe
	msdfgen.exe msdf -font %%a 0x53 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_S.png -autoframe
	msdfgen.exe msdf -font %%a 0x54 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_T.png -autoframe
	msdfgen.exe msdf -font %%a 0x55 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_U.png -autoframe
	msdfgen.exe msdf -font %%a 0x56 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_V.png -autoframe
	msdfgen.exe msdf -font %%a 0x57 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_W.png -autoframe
	msdfgen.exe msdf -font %%a 0x58 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_X.png -autoframe
	msdfgen.exe msdf -font %%a 0x59 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_Y.png -autoframe
	msdfgen.exe msdf -font %%a 0x5A -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Upper_Z.png -autoframe
	msdfgen.exe msdf -font %%a 0x61 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_A.png -autoframe
	msdfgen.exe msdf -font %%a 0x62 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_B.png -autoframe
	msdfgen.exe msdf -font %%a 0x63 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_C.png -autoframe
	msdfgen.exe msdf -font %%a 0x64 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_D.png -autoframe
	msdfgen.exe msdf -font %%a 0x65 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_E.png -autoframe
	msdfgen.exe msdf -font %%a 0x66 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_F.png -autoframe
	msdfgen.exe msdf -font %%a 0x67 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_G.png -autoframe
	msdfgen.exe msdf -font %%a 0x68 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_H.png -autoframe
	msdfgen.exe msdf -font %%a 0x69 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_I.png -autoframe
	msdfgen.exe msdf -font %%a 0x6A -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_J.png -autoframe
	msdfgen.exe msdf -font %%a 0x6B -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_K.png -autoframe
	msdfgen.exe msdf -font %%a 0x6C -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_L.png -autoframe
	msdfgen.exe msdf -font %%a 0x6D -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_M.png -autoframe
	msdfgen.exe msdf -font %%a 0x6E -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_N.png -autoframe
	msdfgen.exe msdf -font %%a 0x6F -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_O.png -autoframe
	msdfgen.exe msdf -font %%a 0x70 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_P.png -autoframe
	msdfgen.exe msdf -font %%a 0x71 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_Q.png -autoframe
	msdfgen.exe msdf -font %%a 0x72 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_R.png -autoframe
	msdfgen.exe msdf -font %%a 0x73 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_S.png -autoframe
	msdfgen.exe msdf -font %%a 0x74 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_T.png -autoframe
	msdfgen.exe msdf -font %%a 0x75 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_U.png -autoframe
	msdfgen.exe msdf -font %%a 0x76 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_V.png -autoframe
	msdfgen.exe msdf -font %%a 0x77 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_W.png -autoframe
	msdfgen.exe msdf -font %%a 0x78 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_X.png -autoframe
	msdfgen.exe msdf -font %%a 0x79 -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_Y.png -autoframe
	msdfgen.exe msdf -font %%a 0x7A -size 32 32 -pxrange 12 -scale 2 -o %%~dpa/Lower_Z.png -autoframe

)

pause


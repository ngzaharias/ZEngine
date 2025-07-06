project "Noesis"
	kind "WindowedApp"
	dependson { }
	pchheader "NoesisPCH.h"
	pchsource "Noesis/NoesisPCH.cpp"
	location "%{wks.location}/Noesis"

	files 
	{ 
		-- code
		"%{wks.location}/../Code/Bindings/Noesis/Noesis/**.h",
		"%{wks.location}/../Code/Bindings/Noesis/Noesis/**.cpp",
		"%{wks.location}/../Code/Bindings/Noesis/Noesis/**.inl",

		-- data
		"%{wks.location}/../Code/Bindings/Noesis/Noesis/**.ttf",
		"%{wks.location}/../Code/Bindings/Noesis/Noesis/**.xaml",
	}

	vpaths 
	{ 
		{ ["Data/*"] = {  
			"Noesis/**.ttf", 
			"Noesis/**.xaml" } },
		{ ["Source/*"] = {  
			"Noesis/**.h", 
			"Noesis/**.cpp", 
			"Noesis/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Include/",
	}

	libdirs 
	{
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Library/",
	}

	links 
	{ 
		"Noesis.lib",
		"NoesisApp.lib",
	}

	prebuildcommands 
	{ 
		--"D:/Projects/C++/ZEngine/3rdParty/NoesisGUI/3.2.8-Indie/Tools/bin2h.exe D:/Projects/C++/ZEngine/Code/Bindings/Noesis/Noesis/Font.ttf > D:/Projects/C++/ZEngine/Code/Bindings/Noesis/Noesis/Font.ttf.bin.h",
	}

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Binary/*.dll $(OutDir)",
	}
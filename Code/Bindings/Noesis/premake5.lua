project "Noesis"
	kind "ConsoleApp"
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
		"%{wks.location}/../3rdParty/glew/2.1.0/Include/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Include/",
	}

	libdirs 
	{
		"%{wks.location}/../3rdParty/glew/2.1.0/Library/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Library/",
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Library/",
	}

	links 
	{ 
		"Noesis.lib",
		"NoesisApp.lib",

		"glew32.lib",
		"glfw3.lib",
	}

	prebuildcommands 
	{ 
		--"D:/Projects/C++/ZEngine/3rdParty/NoesisGUI/3.2.8-Indie/Tools/bin2h.exe D:/Projects/C++/ZEngine/Code/Bindings/Noesis/Noesis/Font.ttf > D:/Projects/C++/ZEngine/Code/Bindings/Noesis/Noesis/Font.ttf.bin.h",
	}

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/glew/2.1.0/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../Code/Bindings/Noesis/Noesis/*.ttf $(OutDir)",
		"{COPY} %{wks.location}/../Code/Bindings/Noesis/Noesis/*.xaml $(OutDir)",
	}
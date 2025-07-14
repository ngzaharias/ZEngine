project "Noesis"
	kind "StaticLib"
	pchheader "NoesisPCH.h"
	pchsource "Noesis/NoesisPCH.cpp"
	location "%{wks.location}/Bindings/Noesis"

	files 
	{ 
		-- code
		"%{wks.location}/../Code/Bindings/Noesis/Noesis/**.h",
		"%{wks.location}/../Code/Bindings/Noesis/Noesis/**.cpp",
		"%{wks.location}/../Code/Bindings/Noesis/Noesis/**.inl",
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
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/assimp/5.2.4/Include/",
		"%{wks.location}/../3rdParty/Freetype/2.13.3/Include/",
		"%{wks.location}/../3rdParty/glew/2.1.0/Include/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/2.6.1/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../3rdParty/yojimbo/1.2.1/Include/",
		"%{wks.location}/../Code/Application/Game/",
		"%{wks.location}/../Code/Application/GameClient/",
		"%{wks.location}/../Code/Application/GameDebug/",
		"%{wks.location}/../Code/Application/GameServer/",
		"%{wks.location}/../Code/Application/GameShared/",
		"%{wks.location}/../Code/Application/GameUI/",
		"%{wks.location}/../Code/Bindings/Steam/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Modules/Hexmap/",
		"%{wks.location}/../Code/Modules/Hidden/",
		"%{wks.location}/../Code/Modules/Softbody/",
		"%{wks.location}/../Code/Modules/Tabletop/",
		"%{wks.location}/../Code/Modules/Voxel/",
	}

	libdirs 
	{
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/assimp/5.2.4/Library/",
		"%{wks.location}/../3rdParty/glew/2.1.0/Library/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Library/",
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Library/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Library/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Library/",
	}

	filter "Debug*"
		libdirs 
		{
			"%{wks.location}/../3rdParty/Freetype/2.13.3/Library/debug",
			"%{wks.location}/../3rdParty/SFML/2.6.1/Library/debug/",
			"%{wks.location}/../3rdParty/PhysX/Library/debug/",
			"%{wks.location}/../3rdParty/yojimbo/1.2.1/Library/debug/",
		}
	filter "Release*"
		libdirs 
		{
			"%{wks.location}/../3rdParty/Freetype/2.13.3/Library/release",
			"%{wks.location}/../3rdParty/SFML/2.6.1/Library/release/",
			"%{wks.location}/../3rdParty/PhysX/Library/release/",
			"%{wks.location}/../3rdParty/yojimbo/1.2.1/Library/release/",
		}
	filter {} -- disable the filter

	links 
	{ 
		"Core",
		"ECS",
		"Engine",
		"GameClient",
		"GameDebug",
		"GameServer",
		"GameShared",
		"GameUI",
		"Imgui",
		"Math",
		"Network",
		"Steam",

		"Hexmap",
		"Hidden",
		"Softbody",
		"Tabletop",
		"Voxel",

		"assimp.lib",
		"freetype.lib",
		"glew32.lib",
		"glfw3.lib",
		"Noesis.lib",
		"NoesisApp.lib",
		"OptickCore.lib",
		"PhysX_64.lib",
		"PhysXCommon_64.lib",
		"PhysXExtensions_static_64.lib",
		"PhysXFoundation_64.lib",
		"PhysXPvdSDK_static_64.lib",
		"steam_api64.lib",
		"yojimbo.lib",

		"Dbghelp.lib",
		"flac.lib",
		"gdi32.lib",
		"ogg.lib",
		"openal32.lib",
		"opengl32.lib",
		"vorbis.lib",
		"vorbisenc.lib",
		"vorbisfile.lib",
		"winmm.lib",
		"ws2_32.lib",
		"winmm.lib",
	}

	filter "Debug*"
		links 
		{ 
			"sfml-audio-d.lib",
		}
	filter "Release*"
		links 
		{ 
			"sfml-audio.lib",
		}
	filter {} -- disable the filter

	filter { "kind:*App" }
		postbuildcommands 
		{ 
			"{COPY} %{wks.location}/../3rdParty/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/assimp/5.2.4/Binary/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/glew/2.1.0/Binary/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Binary/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/SteamworksSDK/Binary/*.dll $(OutDir)",

			"{COPY} %{wks.location}/../steam_appid.txt $(OutDir)",

			"{COPY} %{wks.location}/../Code/Bindings/Noesis/Noesis/*.ttf $(OutDir)",
			"{COPY} %{wks.location}/../Code/Bindings/Noesis/Noesis/*.xaml $(OutDir)",
		}
	filter { "kind:*App", "Debug*" }
		postbuildcommands 
		{
			"{COPY} %{wks.location}/../3rdParty/Freetype/2.13.3/Binary/debug/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/PhysX/Binary/debug/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/SFML/2.6.1/Binary/debug/*.dll $(OutDir)",
		}
	filter { "kind:*App", "Release*" }
		postbuildcommands 
		{
			"{COPY} %{wks.location}/../3rdParty/Freetype/2.13.3/Binary/release/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/PhysX/Binary/release/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/SFML/2.6.1/Binary/release/*.dll $(OutDir)",
		}
	filter {} -- disable the filter
project "Game"

	--kind "ConsoleApp"
	kind "WindowedApp"
	filter "*_Editor"
		kind "StaticLib"
	filter {} -- disable the filter

	pchheader "GamePCH.h"
	pchsource "Game/GamePCH.cpp"
	location "%{wks.location}/Game"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Game/**.h", 
			"Game/**.cpp", 
			"Game/**.inl" } },
		 
		{ ["Resource/*"] = {  
			"Resource/**.*" } },
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
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/2.6.1/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../3rdParty/yojimbo/1.2.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/Game/",
		"%{wks.location}/../Code/GameClient/",
		"%{wks.location}/../Code/GameDebug/",
		"%{wks.location}/../Code/GameServer/",
		"%{wks.location}/../Code/GameShared/",
		"%{wks.location}/../Code/GameUI/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/../Code/Input/",
		"%{wks.location}/../Code/Math/",
		"%{wks.location}/../Code/Network/",
		"%{wks.location}/../Code/Bindings/Steam/",
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
			"{COPY} %{wks.location}/../3rdParty/SteamworksSDK/Binary/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../steam_appid.txt $(OutDir)",
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

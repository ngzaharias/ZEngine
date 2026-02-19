project "Game"
	kind "WindowedApp"
	filter "Debug*"
		kind "ConsoleApp"
	filter "*_Editor"
		kind "StaticLib"
	filter {}

	pchheader "GamePCH.h"
	pchsource "Game/GamePCH.cpp"
	location "%{wks.location}/Application/Game"

	files 
	{ 
		"%{wks.location}/../Code/Application/Game/*.natvis",
		"%{wks.location}/../Code/Application/Game/premake5.*",
		"%{wks.location}/../Code/Application/Game/Game/**.h",
		"%{wks.location}/../Code/Application/Game/Game/**.cpp",
		"%{wks.location}/../Code/Application/Game/Game/**.inl",
		"%{wks.location}/../Code/Application/Game/Resource/**.*",
	}

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
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/2.6.1/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../Code/Application/Game/",
		"%{wks.location}/../Code/Application/GameClient/",
		"%{wks.location}/../Code/Application/GameDebug/",
		"%{wks.location}/../Code/Application/GameServer/",
		"%{wks.location}/../Code/Application/GameUI/",
		"%{wks.location}/../Code/Bindings/Steam/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Framework/Render/",
		"%{wks.location}/../Code/Modules/Camera/",
		"%{wks.location}/../Code/Modules/ClientCursor/",
		"%{wks.location}/../Code/Modules/ClientHidden/",
		"%{wks.location}/../Code/Modules/ClientNetwork/",
		"%{wks.location}/../Code/Modules/Container/",
		"%{wks.location}/../Code/Modules/Hexmap/",
		"%{wks.location}/../Code/Modules/ServerCursor/",
		"%{wks.location}/../Code/Modules/ServerHidden/",
		"%{wks.location}/../Code/Modules/ServerNetwork/",
		"%{wks.location}/../Code/Modules/SharedCursor/",
		"%{wks.location}/../Code/Modules/SharedHidden/",
		"%{wks.location}/../Code/Modules/SharedNetwork/",
		"%{wks.location}/../Code/Modules/Softbody/",
		"%{wks.location}/../Code/Modules/Tabletop/",
		"%{wks.location}/../Code/Modules/Tactics/",
		"%{wks.location}/../Code/Modules/Tilemap/",
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
		}
	filter "Release*"
		libdirs 
		{
			"%{wks.location}/../3rdParty/Freetype/2.13.3/Library/release",
			"%{wks.location}/../3rdParty/SFML/2.6.1/Library/release/",
			"%{wks.location}/../3rdParty/PhysX/Library/release/",
		}
	filter {} -- disable the filter

	links 
	{ 
		-- Application
		"GameClient",
		"GameDebug",
		"GameServer",
		"GameState",
		"GameUI",

		-- Bindings
		"Steam",

		-- Framework
		"Core",
		"ECS",
		"Engine",
		"Imgui",
		"Math",
		"Network",
		"Render",

		-- Modules
		"Camera",
		"ClientCursor",
		"ClientHidden",
		"ClientNetwork",
		"Container",
		"Hexmap",
		"ServerCursor",
		"ServerHidden",
		"ServerNetwork",
		"SharedCursor",
		"SharedHidden",
		"SharedNetwork",
		"Softbody",
		"Tabletop",
		"Tactics",
		"Tilemap",
		"Voxel",

		-- 3rdParty
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
			"{COPY} %{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Binary/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/SteamworksSDK/Binary/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../Code/Application/Game/Resource/ClientIcon.png $(OutDir)",
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

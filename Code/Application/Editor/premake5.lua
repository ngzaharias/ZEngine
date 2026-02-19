project "Editor"

	kind "None"
	filter "*_Editor"
		kind "ConsoleApp"
	filter {} -- disable the filter

	pchheader "EditorPCH.h"
	pchsource "Editor/EditorPCH.cpp"
	location "%{wks.location}/Application/Editor"

	files 
	{ 
		"%{wks.location}/../Code/Application/Editor/*.natvis",
		"%{wks.location}/../Code/Application/Editor/premake5.*",
		"%{wks.location}/../Code/Application/Editor/Editor/**.h",
		"%{wks.location}/../Code/Application/Editor/Editor/**.cpp",
		"%{wks.location}/../Code/Application/Editor/Editor/**.inl",
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Editor/**.h", 
			"Editor/**.cpp", 
			"Editor/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/assimp/5.2.4/Include/",
		"%{wks.location}/../3rdParty/Freetype/2.13.3/Include/",
		"%{wks.location}/../3rdParty/glew/2.1.0/Include/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/imguizmo/1.83/Include/",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/2.6.1/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../Code/Application/Editor/",
		"%{wks.location}/../Code/Application/Game/",
		"%{wks.location}/../Code/Application/GameClient/",
		"%{wks.location}/../Code/Application/GameDebug/",
		"%{wks.location}/../Code/Application/GameServer/",
		"%{wks.location}/../Code/Application/GameUI/",
		"%{wks.location}/../Code/Bindings/Steam/",
		"%{wks.location}/../Code/Editors/FlipbookEditor/",
		"%{wks.location}/../Code/Editors/Icons/",
		"%{wks.location}/../Code/Editors/InputEditor/",
		"%{wks.location}/../Code/Editors/Inspector/",
		"%{wks.location}/../Code/Editors/Outliner/",
		"%{wks.location}/../Code/Editors/SpriteEditor/",
		"%{wks.location}/../Code/Editors/TableEditor/",
		"%{wks.location}/../Code/Editors/TextureEditor/",
		"%{wks.location}/../Code/Editors/ThemeEditor/",
		"%{wks.location}/../Code/Editors/TrajectoryEditor/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Framework/Render/",
		"%{wks.location}/../Code/Modules/Camera/",
		"%{wks.location}/../Code/Modules/ClientHidden/",
		"%{wks.location}/../Code/Modules/Container/",
		"%{wks.location}/../Code/Modules/GameState/",
		"%{wks.location}/../Code/Modules/Softbody/",
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
		"Game",
		"GameClient",
		"GameDebug",
		"GameServer",
		"GameUI",

		-- Bindings
		"Steam",

		-- Editors
		"FlipbookEditor",
		"Icons",
		"InputEditor",
		"Inspector",
		"Outliner",
		"SpriteEditor",
		"TableEditor",
		"TextureEditor",
		"ThemeEditor",
		"TrajectoryEditor",

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
		"GameState",
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
			"sfml-graphics-d.lib",
			"sfml-network-d.lib",
			"sfml-system-d.lib",
			"sfml-window-d.lib",
		}
	filter "Release*"
		links 
		{ 
			"sfml-audio.lib",
			"sfml-graphics.lib",
			"sfml-network.lib",
			"sfml-system.lib",
			"sfml-window.lib",
		}
	filter {} -- disable the filter

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/assimp/5.2.4/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/glew/2.1.0/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/SteamworksSDK/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../steam_appid.txt $(OutDir)",
	}

	filter "Debug*"
		postbuildcommands 
		{
			"{COPY} %{wks.location}/../3rdParty/Freetype/2.13.3/Binary/debug/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/PhysX/Binary/debug/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/SFML/2.6.1/Binary/debug/*.dll $(OutDir)",
		}
	filter "Release*"
		postbuildcommands 
		{
			"{COPY} %{wks.location}/../3rdParty/Freetype/2.13.3/Binary/release/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/PhysX/Binary/release/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/SFML/2.6.1/Binary/release/*.dll $(OutDir)",
		}
	filter {} -- disable the filter
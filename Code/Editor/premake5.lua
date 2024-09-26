project "Editor"

	kind "None"
	filter "*_Editor"
		kind "ConsoleApp"
	filter {} -- disable the filter

	dependson { "Core", "ECS", "Engine", "Game", "GameClient", "GameDebug", "GameServer", "GameShared", "Imgui", "Math", "Network" }
	pchheader "EditorPCH.h"
	pchsource "Editor/EditorPCH.cpp"
	location "%{wks.location}/Editor"

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
		"%{wks.location}/../3rdParty/imgui/1.87/Include/",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/Include/",
		"%{wks.location}/../3rdParty/yojimbo/1.2.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Editor/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/Game/",
		"%{wks.location}/../Code/GameClient/",
		"%{wks.location}/../Code/GameDebug/",
		"%{wks.location}/../Code/GameServer/",
		"%{wks.location}/../Code/GameShared/",
		"%{wks.location}/../Code/GameUI/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/../Code/Math/",
		"%{wks.location}/../Code/Network/",
	}

	libdirs 
	{
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/assimp/5.2.4/Library/",
		"%{wks.location}/../3rdParty/glew/2.1.0/Library/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Library/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Library/",
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/ECS/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Engine/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Game/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/GameClient/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/GameDebug/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/GameServer/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/GameShared/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/GameUI/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Imgui/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Math/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Network/%{cfg.buildcfg}_%{cfg.platform}/",
	}

	filter "Debug*"
		libdirs 
		{
			"%{wks.location}/../3rdParty/Freetype/2.13.3/Library/debug",
			"%{wks.location}/../3rdParty/SFML/Library/debug/",
			"%{wks.location}/../3rdParty/PhysX/Library/debug/",
			"%{wks.location}/../3rdParty/yojimbo/1.2.1/Library/debug/",
		}
	filter "Release*"
		libdirs 
		{
			"%{wks.location}/../3rdParty/Freetype/2.13.3/Library/release",
			"%{wks.location}/../3rdParty/SFML/Library/release/",
			"%{wks.location}/../3rdParty/PhysX/Library/release/",
			"%{wks.location}/../3rdParty/yojimbo/1.2.1/Library/release/",
		}
	filter {} -- disable the filter

	links 
	{ 
		"Core.lib",
		"ECS.lib",
		"Engine.lib",
		"Game.lib",
		"GameClient.lib",
		"GameDebug.lib",
		"GameServer.lib",
		"GameShared.lib",
		"GameUI.lib",
		"Imgui.lib",
		"Math.lib",
		"Network.lib",

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

		"yojimbo.lib",

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

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/assimp/5.2.4/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/glew/2.1.0/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
	}

	filter "Debug*"
		postbuildcommands 
		{
			"{COPY} %{wks.location}/../3rdParty/Freetype/2.13.3/Binary/debug/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/PhysX/Binary/debug/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/SFML/Binary/debug/*.dll $(OutDir)",
		}
	filter "Release*"
		postbuildcommands 
		{
			"{COPY} %{wks.location}/../3rdParty/Freetype/2.13.3/Binary/release/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/PhysX/Binary/release/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/SFML/Binary/release/*.dll $(OutDir)",
		}
	filter {} -- disable the filter
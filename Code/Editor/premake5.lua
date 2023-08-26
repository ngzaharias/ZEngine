project "Editor"
	kind "ConsoleApp"
	dependson { "Core", "Engine", "Game", "Editor_gen", "Imgui" }
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
		"%{wks.location}/../3rdParty/glew/2.1.0/Include/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/imgui/1.87/Include/",
		"%{wks.location}/../3rdParty/imgui-nodes/0.5/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/Include/",
		"%{wks.location}/../3rdParty/tinyxml2/9.0.0/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/Editor/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/Game/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/Generated/Editor/",
	}

	libdirs 
	{
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/glew/2.1.0/Library/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Library/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Library/",
		"%{wks.location}/../3rdParty/PhysX/Library/%{cfg.buildcfg}/",
		"%{wks.location}/../3rdParty/SFML/Library/%{cfg.buildcfg}/",
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Engine/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Imgui/%{cfg.buildcfg}_%{cfg.platform}/",
	}

	links 
	{ 
		"Core.lib",
		"Engine.lib",
		"Imgui.lib",

		"glew32.lib",
		"glfw3.lib",

		"OptickCore.lib",

		"PhysX_64.lib",
		"PhysXCommon_64.lib",
		"PhysXExtensions_static_64.lib",
		"PhysXFoundation_64.lib",
		"PhysXPvdSDK_static_64.lib",

		"flac.lib",
		"freetype.lib",
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

	filter "Debug"
		links 
		{ 
			"sfml-audio-d.lib",
		}
	filter "Release"
		links 
		{ 
			"sfml-audio.lib",
		}
	filter {} -- disable the filter

	buildcommands 
	{ 
		"cd %{wks.location}/../Assets/Shaders/ & call generate.bat" 
	}

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/glew/2.1.0/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/PhysX/Binary/%{cfg.buildcfg}/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/SFML/Binary/%{cfg.buildcfg}/*.dll $(OutDir)",
	}


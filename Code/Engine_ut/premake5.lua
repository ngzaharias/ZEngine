project "Engine_ut"
	kind "ConsoleApp"
	dependson { "Core", "ECS", "Engine", "Math" }
	location "%{wks.location}/Engine_ut"

	defines { "Z_UNIT_TEST" }

	disablewarnings 
	{ 
		"6237",
		"6319", 
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Engine_ut/**.h", 
			"Engine_ut/**.cpp", 
			"Engine_ut/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/SFML/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/Math/",
		"%{wks.location}/../Code/SteamBinding/",
	}

	libdirs 
	{ 
		"%{wks.location}/../3rdParty/optick/1.3.1/Library/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Library/",
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/ECS/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Engine/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Math/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/SteamBinding/%{cfg.buildcfg}_%{cfg.platform}/",
	}

	filter "Debug*"
		libdirs 
		{
			"%{wks.location}/../3rdParty/SFML/Library/debug/",
		}
	filter "Release*"
		libdirs 
		{
			"%{wks.location}/../3rdParty/SFML/Library/release/",
		}
	filter {} -- disable the filter
	
	links 
	{ 
		"Core.lib",
		"ECS.lib",
		"Engine.lib",
		"Math.lib",
		"SteamBinding.lib",

		"OptickCore.lib",

		"steam_api64.lib",
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
		"{COPY} %{wks.location}/../3rdParty/SteamworksSDK/Binary/*.dll $(OutDir)",
	}

	filter "Debug*"
		postbuildcommands 
		{
			"{COPY} %{wks.location}/../3rdParty/PhysX/Binary/debug/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/SFML/Binary/debug/*.dll $(OutDir)",
		}
	filter "Release*"
		postbuildcommands 
		{
			"{COPY} %{wks.location}/../3rdParty/PhysX/Binary/release/*.dll $(OutDir)",
			"{COPY} %{wks.location}/../3rdParty/SFML/Binary/release/*.dll $(OutDir)",
		}
	filter {} -- disable the filter
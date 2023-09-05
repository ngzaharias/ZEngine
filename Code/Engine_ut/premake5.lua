project "Engine_ut"
	kind "ConsoleApp"
	dependson { "Core", "ECS", "Engine" }
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
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/SFML/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
	}

	libdirs 
	{ 
		"%{wks.location}/../3rdParty/optick/1.3.1/Library/",
		"%{wks.location}/../3rdParty/SFML/Library/%{cfg.buildcfg}/",
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/ECS/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Engine/%{cfg.buildcfg}_%{cfg.platform}/",
	}
	
	links 
	{ 
		"Core.lib",
		"ECS.lib",
		"Engine.lib",
		"OptickCore.lib",
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

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/assimp/5.2.4/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/SFML/Binary/%{cfg.buildcfg}/*.dll $(OutDir)",
	}
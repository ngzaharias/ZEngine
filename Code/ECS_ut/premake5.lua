project "ECS_ut"
	kind "ConsoleApp"
	dependson { "Core", "ECS" }
	location "%{wks.location}/ECS_ut"

	defines { "Z_UNIT_TEST" }

	disablewarnings 
	{ 
		"6237",
		"6319", 
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"ECS_ut/**.h", 
			"ECS_ut/**.cpp", 
			"ECS_ut/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
	}

	libdirs 
	{ 
		"%{wks.location}/../3rdParty/optick/1.3.1/Library/",
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/ECS/%{cfg.buildcfg}_%{cfg.platform}/",
	}
	
	links 
	{ 
		"Core.lib",
		"ECS.lib",
		"OptickCore.lib",
	}

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
	}

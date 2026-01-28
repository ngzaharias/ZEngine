project "ECS_ut"
	kind "ConsoleApp"
	location "%{wks.location}/Framework/ECS_ut"

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
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Network/",
	}

	libdirs 
	{ 
		"%{wks.location}/../3rdParty/optick/1.3.1/Library/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Library/",
	}
	
	links 
	{ 
		-- Bindings
		"Steam",

		-- Framework
		"Core",
		"ECS",
		"Network",	
		
		-- 3rdParty
		"OptickCore.lib",
		"steam_api64.lib",
	}

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/SteamworksSDK/Binary/*.dll $(OutDir)",
	}

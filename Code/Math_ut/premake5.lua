project "Math_ut"
	kind "ConsoleApp"
	dependson { "Math" }
	location "%{wks.location}/Math_ut"

	defines { "Z_UNIT_TEST" }

	disablewarnings 
	{ 
		"6237",
		"6319", 
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Math_ut/**.h", 
			"Math_ut/**.cpp", 
			"Math_ut/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/Math/",
	}

	libdirs 
	{ 
		"%{wks.location}/Build/Core/%{cfg.buildcfg}_%{cfg.platform}/",
		"%{wks.location}/Build/Math/%{cfg.buildcfg}_%{cfg.platform}/" 
	}
	
	links 
	{ 
		"Core.lib",
		"Math.lib" 
	}

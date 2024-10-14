project "SteamBinding"
	kind "StaticLib"
	location "%{wks.location}/SteamBinding"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"SteamBinding/**.h", 
			"SteamBinding/**.cpp", 
			"SteamBinding/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../Code/SteamBinding/",
	}
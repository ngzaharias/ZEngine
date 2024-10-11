project "SteamBinding"
	kind "StaticLib"
	dependson { }
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
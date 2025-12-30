project "Steam"
	kind "StaticLib"
	location "%{wks.location}/Bindings/Steam"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Steam/**.h", 
			"Steam/**.cpp", 
			"Steam/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../Code/Bindings/Steam/",
		"%{wks.location}/../Code/Framework/Core/",
	}
project "Network"
	kind "StaticLib"
	dependson { "Core", "Math" }
	location "%{wks.location}/Network"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Network/**.h", 
			"Network/**.cpp", 
			"Network/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/yojimbo/1.2.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/Network/",
	}
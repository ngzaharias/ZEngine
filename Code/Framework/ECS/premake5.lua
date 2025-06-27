project "ECS"
	kind "StaticLib"
	location "%{wks.location}/Framework/ECS"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"ECS/**.h", 
			"ECS/**.cpp", 
			"ECS/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/spdlog/1.10.0/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
	}

	links 
	{ 
		"Core", 
	}

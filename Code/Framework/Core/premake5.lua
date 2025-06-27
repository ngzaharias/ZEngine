project "Core"
	kind "StaticLib"
	location "%{wks.location}/Framework/Core"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Core/**.h", 
			"Core/**.cpp", 
			"Core/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/spdlog/1.10.0/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/Math/",
	}

	links 
	{ 
		"Math",
	}

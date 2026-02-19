project "GameState"
	kind "StaticLib"
	location "%{wks.location}/Modules/GameState"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"GameState/**.h", 
			"GameState/**.cpp", 
			"GameState/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Modules/GameState/",
	}
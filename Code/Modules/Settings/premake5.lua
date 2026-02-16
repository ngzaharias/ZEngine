project "Settings"
	kind "StaticLib"
	location "%{wks.location}/Modules/Settings"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Settings/**.h", 
			"Settings/**.cpp", 
			"Settings/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Modules/Settings/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
	}
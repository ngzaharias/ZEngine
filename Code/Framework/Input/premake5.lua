project "Input"
	kind "StaticLib"
	location "%{wks.location}/Framework/Input"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Input/**.h", 
			"Input/**.cpp", 
			"Input/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
	}

	links 
	{ 
		"Core",
		"Math",
	}

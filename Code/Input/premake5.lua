project "Input"
	kind "StaticLib"
	location "%{wks.location}/Input"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Input/**.h", 
			"Input/**.cpp", 
			"Input/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/Input/",
		"%{wks.location}/../Code/Math/",
	}

	links 
	{ 
		"Core",
		"Math",
	}

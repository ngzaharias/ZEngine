project "Math"
	kind "StaticLib"
	location "%{wks.location}/Math"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Math/**.h", 
			"Math/**.cpp", 
			"Math/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../Code/Math/",
	}
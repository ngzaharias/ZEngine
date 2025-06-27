project "Math"
	kind "StaticLib"
	location "%{wks.location}/Framework/Math"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Math/**.h", 
			"Math/**.cpp", 
			"Math/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../Code/Framework/Math/",
	}
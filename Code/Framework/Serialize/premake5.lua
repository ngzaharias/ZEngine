project "Serialize"
	kind "StaticLib"
	location "%{wks.location}/Framework/Serialize"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Serialize/**.h", 
			"Serialize/**.cpp", 
			"Serialize/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Serialize/",
	}
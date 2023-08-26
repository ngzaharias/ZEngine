project "Kodgen"
	kind "StaticLib"
	dependson { }
	location "%{wks.location}/Kodgen"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Kodgen/**.h", 
			"Kodgen/**.cpp", 
			"Kodgen/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/clang-c/Include/",
		"%{wks.location}/../3rdParty/toml11/",
		"%{wks.location}/../Code/Kodgen/",
	}
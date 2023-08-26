project "ECS"
	kind "StaticLib"
	dependson { "Core" }
	location "%{wks.location}/ECS"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"ECS/**.h", 
			"ECS/**.cpp", 
			"ECS/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/spdlog/1.10.0/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
	}
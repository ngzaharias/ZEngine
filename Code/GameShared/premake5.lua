project "GameShared"
	kind "StaticLib"
	dependson { "Core", "ECS", "Engine", "Network" }
	pchheader "GameSharedPCH.h"
	pchsource "GameShared/GameSharedPCH.cpp"
	location "%{wks.location}/GameShared"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"GameShared/**.h", 
			"GameShared/**.cpp", 
			"GameShared/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/GameShared/",
		"%{wks.location}/../Code/Network/",
	}
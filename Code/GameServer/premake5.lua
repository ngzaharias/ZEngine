project "GameServer"
	kind "StaticLib"
	dependson { "Core", "ECS", "Engine", "GameShared", "Math", "Network" }
	pchheader "GameServerPCH.h"
	pchsource "GameServer/GameServerPCH.cpp"
	location "%{wks.location}/GameServer"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"GameServer/**.h", 
			"GameServer/**.cpp", 
			"GameServer/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/yojimbo/1.2.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/GameServer/",
		"%{wks.location}/../Code/GameShared/",
		"%{wks.location}/../Code/Math/",
		"%{wks.location}/../Code/Network/",
	}
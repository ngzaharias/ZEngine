project "GameServer"
	kind "StaticLib"
	pchheader "GameServerPCH.h"
	pchsource "GameServer/GameServerPCH.cpp"
	location "%{wks.location}/Application/GameServer"

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
		"%{wks.location}/../Code/Application/GameServer/",
		"%{wks.location}/../Code/Application/GameShared/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Network/",
	}

	links 
	{ 
		"Core", 
		"ECS", 
		"Engine", 
		"GameShared", 
		"Math", 
		"Network" 
	}

project "GameClient"
	kind "StaticLib"
	dependson { "Core", "ECS", "Engine", "GameShared", "Imgui", "Network" }
	pchheader "GameClientPCH.h"
	pchsource "GameClient/GameClientPCH.cpp"
	location "%{wks.location}/GameClient"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"GameClient/**.h", 
			"GameClient/**.cpp", 
			"GameClient/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.87/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/Include/",
		"%{wks.location}/../3rdParty/yojimbo/1.2.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/GameClient/",
		"%{wks.location}/../Code/GameShared/",
		"%{wks.location}/../Code/Network/",
	}
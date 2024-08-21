project "GameDebug"
	kind "StaticLib"
	dependson { "Core", "Engine", "GameClient", "GameServer", "GameShared", "GameUI", "Imgui", "Math", "Network" }
	pchheader "GameDebugPCH.h"
	pchsource "GameDebug/GameDebugPCH.cpp"
	location "%{wks.location}/GameDebug"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"GameDebug/**.h", 
			"GameDebug/**.cpp", 
			"GameDebug/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.87/Include/",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/yojimbo/1.2.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/Game/",
		"%{wks.location}/../Code/GameClient/",
		"%{wks.location}/../Code/GameDebug/",
		"%{wks.location}/../Code/GameServer/",
		"%{wks.location}/../Code/GameShared/",
		"%{wks.location}/../Code/GameUI/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/../Code/Math/",
		"%{wks.location}/../Code/Network/",
	}
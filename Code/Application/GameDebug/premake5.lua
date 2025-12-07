project "GameDebug"
	kind "StaticLib"
	pchheader "GameDebugPCH.h"
	pchsource "GameDebug/GameDebugPCH.cpp"
	location "%{wks.location}/Application/GameDebug"

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
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/imguizmo/1.83/Include/",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../Code/Application/Game/",
		"%{wks.location}/../Code/Application/GameClient/",
		"%{wks.location}/../Code/Application/GameDebug/",
		"%{wks.location}/../Code/Application/GameServer/",
		"%{wks.location}/../Code/Application/GameShared/",
		"%{wks.location}/../Code/Application/GameUI/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Modules/Container/",
		"%{wks.location}/../Code/Modules/Hexmap/",
		"%{wks.location}/../Code/Modules/Hidden/",
		"%{wks.location}/../Code/Modules/Softbody/",
		"%{wks.location}/../Code/Modules/Tabletop/",
		"%{wks.location}/../Code/Modules/Voxel/",
	}

	links 
	{ 
		"Core", 
		"Engine", 
		"GameClient", 
		"GameServer", 
		"GameShared", 
		"GameUI", 
		"Imgui", 
		"Math", 
		"Network",

		"Container",
		"Hexmap",
		"Hidden",
		"Softbody",
		"Tabletop",
		"Voxel",
	}

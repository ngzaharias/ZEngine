project "GameClient"
	kind "StaticLib"
	pchheader "GameClientPCH.h"
	pchsource "GameClient/GameClientPCH.cpp"
	location "%{wks.location}/Application/GameClient"

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
		"%{wks.location}/../3rdParty/glew/2.1.0/Include/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/2.6.1/Include/",
		"%{wks.location}/../Code/Application/GameClient/",
		"%{wks.location}/../Code/Application/GameShared/",
		"%{wks.location}/../Code/Application/GameUI/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Modules/Hexmap/",
		"%{wks.location}/../Code/Modules/Hidden/",
		"%{wks.location}/../Code/Modules/Softbody/",
		"%{wks.location}/../Code/Modules/Tabletop/",
		"%{wks.location}/../Code/Modules/Tactics/",
		"%{wks.location}/../Code/Modules/Tilemap/",
		"%{wks.location}/../Code/Modules/Voxel/",
	}

	links 
	{ 
		"Core", 
		"ECS", 
		"Engine", 
		"GameShared", 
		"Imgui", 
		"Math", 
		"Network",
	}

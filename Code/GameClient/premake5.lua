project "GameClient"
	kind "StaticLib"
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
		"%{wks.location}/../3rdParty/glew/2.1.0/Include/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/2.6.1/Include/",
		"%{wks.location}/../3rdParty/yojimbo/1.2.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/GameClient/",
		"%{wks.location}/../Code/GameShared/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/../Code/Input/",
		"%{wks.location}/../Code/Math/",
		"%{wks.location}/../Code/Network/",
		"%{wks.location}/../Code/Modules/Hexmap/",
		"%{wks.location}/../Code/Modules/Hidden/",
		"%{wks.location}/../Code/Modules/Softbody/",
		"%{wks.location}/../Code/Modules/Tabletop/",
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

project "GameUI"
	kind "StaticLib"
	pchheader "GameUIPCH.h"
	pchsource "GameUI/GameUIPCH.cpp"
	location "%{wks.location}/GameUI"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"GameUI/**.h", 
			"GameUI/**.cpp", 
			"GameUI/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/GameClient/",
		"%{wks.location}/../Code/GameUI/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/../Code/Input/",
		"%{wks.location}/../Code/Math/",
		"%{wks.location}/../Code/Modules/Hidden/",
	}

	links 
	{ 
		"Core", 
		"ECS", 
		"Engine", 
		"GameClient", 
		"Imgui", 
		"Math",
	}

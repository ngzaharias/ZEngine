project "GameUI"
	kind "StaticLib"
	dependson { "Core", "ECS", "Engine", "Imgui", "Math" }
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
		"%{wks.location}/../3rdParty/imgui/1.87/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/GameUI/",
		"%{wks.location}/../Code/Math/",
	}
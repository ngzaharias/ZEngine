project "GameUI"
	kind "StaticLib"
	pchheader "GameUIPCH.h"
	pchsource "GameUI/GameUIPCH.cpp"
	location "%{wks.location}/Application/GameUI"

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
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Application/GameClient/",
		"%{wks.location}/../Code/Application/GameUI/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Modules/Camera/",
		"%{wks.location}/../Code/Modules/Hidden/",
	}

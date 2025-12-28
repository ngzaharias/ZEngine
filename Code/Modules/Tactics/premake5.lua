project "Tactics"
	kind "StaticLib"
	pchheader "TacticsPCH.h"
	pchsource "Tactics/TacticsPCH.cpp"
	location "%{wks.location}/Modules/Tactics"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Tactics/**.h", 
			"Tactics/**.cpp", 
			"Tactics/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Modules/Tactics/",
		"%{wks.location}/../Code/Modules/Tilemap/",
	}
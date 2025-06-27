project "Tabletop"
	kind "StaticLib"
	pchheader "TabletopPCH.h"
	pchsource "Tabletop/TabletopPCH.cpp"
	location "%{wks.location}/Modules/Tabletop"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Tabletop/**.h", 
			"Tabletop/**.cpp", 
			"Tabletop/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Modules/Tabletop/",
	}

	links 
	{ 
		"Core", 
		"ECS", 
		"Engine", 
		"Imgui", 
		"Math", 
	}
project "Hidden"
	kind "StaticLib"
	pchheader "HiddenPCH.h"
	pchsource "Hidden/HiddenPCH.cpp"
	location "%{wks.location}/Modules/Hidden"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Hidden/**.h", 
			"Hidden/**.cpp", 
			"Hidden/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/2.6.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/../Code/Math/",
		"%{wks.location}/../Code/Modules/Hidden/",
	}
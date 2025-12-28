project "Camera"
	kind "StaticLib"
	pchheader "CameraPCH.h"
	pchsource "Camera/CameraPCH.cpp"
	location "%{wks.location}/Framework/Camera"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Camera/**.h", 
			"Camera/**.cpp", 
			"Camera/**.inl" } },
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
		"%{wks.location}/../Code/Framework/ImGui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Modules/Camera/",
	}

	links 
	{ 
		"Core", 
		"ECS", 
		"Engine", 
		"ImGui", 
		"Math", 
	}
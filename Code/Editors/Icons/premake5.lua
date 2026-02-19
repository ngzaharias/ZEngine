project "Icons"
	kind "StaticLib"
	pchheader "IconsPCH.h"
	pchsource "Icons/IconsPCH.cpp"
	location "%{wks.location}/Editors/Icons"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Icons/**.h", 
			"Icons/**.cpp", 
			"Icons/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Editors/Icons/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/ImGui/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Modules/GameState/",
	}
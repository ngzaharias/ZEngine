project "ThemeEditor"
	kind "StaticLib"
	pchheader "ThemeEditorPCH.h"
	pchsource "ThemeEditor/ThemeEditorPCH.cpp"
	location "%{wks.location}/Editors/ThemeEditor"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"ThemeEditor/**.h", 
			"ThemeEditor/**.cpp", 
			"ThemeEditor/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Editors/ThemeEditor/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/ImGui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Modules/GameState/",
	}
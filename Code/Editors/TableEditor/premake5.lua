project "TableEditor"
	kind "StaticLib"
	pchheader "TableEditorPCH.h"
	pchsource "TableEditor/TableEditorPCH.cpp"
	location "%{wks.location}/Editors/TableEditor"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"TableEditor/**.h", 
			"TableEditor/**.cpp", 
			"TableEditor/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Editors/TableEditor/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/ImGui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
	}
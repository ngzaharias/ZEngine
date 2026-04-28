project "SpellEditor"
	kind "StaticLib"
	pchheader "SpellEditorPCH.h"
	pchsource "SpellEditor/SpellEditorPCH.cpp"
	location "%{wks.location}/Editors/SpellEditor"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"SpellEditor/**.h", 
			"SpellEditor/**.cpp", 
			"SpellEditor/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../Code/Editors/NodeGraph/",
		"%{wks.location}/../Code/Editors/SpellEditor/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Serialize/",
		"%{wks.location}/../Code/Modules/GameState/",
		"%{wks.location}/../Code/Modules/Spellcraft/",
	}
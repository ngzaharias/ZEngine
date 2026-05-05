project "DebugCrafting"
	kind "StaticLib"
	pchheader "DebugCraftingPCH.h"
	pchsource "DebugCrafting/DebugCraftingPCH.cpp"
	location "%{wks.location}/Modules/DebugCrafting"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"DebugCrafting/**.h", 
			"DebugCrafting/**.cpp", 
			"DebugCrafting/**.inl" } },
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
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Serialize/",
		"%{wks.location}/../Code/Debuggers/DebugCrafting/",
		"%{wks.location}/../Code/Modules/Crafting/",
	}
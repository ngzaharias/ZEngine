project "SharedSpellcraft"
	kind "StaticLib"
	pchheader "SpellcraftPCH.h"
	pchsource "SharedSpellcraft/SpellcraftPCH.cpp"
	location "%{wks.location}/Modules/Spellcraft/SharedSpellcraft"

	local root = path.getdirectory(_SCRIPT)
	files 
	{ 
		root .. "/premake5.lua",
		root .. "/*.natvis",
		root .. "/%{prj.name}/**.h",
		root .. "/%{prj.name}/**.cpp",
		root .. "/%{prj.name}/**.inl",
		root .. "/Resource/**",
	}
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"SharedSpellcraft/**.h", 
			"SharedSpellcraft/**.cpp", 
			"SharedSpellcraft/**.inl" } },
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
		"%{wks.location}/../Code/Framework/Serialize/",
		"%{wks.location}/../Code/Modules/GameState/SharedGameState/",
		"%{wks.location}/../Code/Modules/Spellcraft/SharedSpellcraft/",
	}
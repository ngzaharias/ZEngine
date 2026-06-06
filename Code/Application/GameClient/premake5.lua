project "GameClient"
	kind "StaticLib"
	pchheader "GameClientPCH.h"
	pchsource "GameClient/GameClientPCH.cpp"
	location "%{wks.location}/Application/GameClient"

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
			"GameClient/**.h", 
			"GameClient/**.cpp", 
			"GameClient/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/glew/2.1.0/Include/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/NoesisGUI/3.2.8-Indie/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/2.6.1/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../Code/Application/GameClient/",
		"%{wks.location}/../Code/Application/GameUI/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Icons/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Framework/Render/",
		"%{wks.location}/../Code/Framework/Serialize/",
		"%{wks.location}/../Code/Modules/Camera/SharedCamera/",
		"%{wks.location}/../Code/Modules/Crafting/SharedCrafting/",
		"%{wks.location}/../Code/Modules/Cursor/ClientCursor/",
		"%{wks.location}/../Code/Modules/Cursor/SharedCursor/",
		"%{wks.location}/../Code/Modules/Hexmap/SharedHexmap/",
		"%{wks.location}/../Code/Modules/Hidden/ClientHidden/",
		"%{wks.location}/../Code/Modules/Hidden/SharedHidden/",
		"%{wks.location}/../Code/Modules/Inventory/SharedInventory/",
		"%{wks.location}/../Code/Modules/Network/ClientNetwork/",
		"%{wks.location}/../Code/Modules/Network/SharedNetwork/",
		"%{wks.location}/../Code/Modules/Softbody/SharedSoftbody/",
		"%{wks.location}/../Code/Modules/Spellcraft/SharedSpellcraft/",
		"%{wks.location}/../Code/Modules/Tabletop/SharedTabletop/",
		"%{wks.location}/../Code/Modules/Tactics/ClientTactics/",
		"%{wks.location}/../Code/Modules/Tactics/SharedTactics/",
		"%{wks.location}/../Code/Modules/Tilemap/SharedTilemap/",
		"%{wks.location}/../Code/Modules/Voxel/ClientVoxel/",
		"%{wks.location}/../Code/Modules/Voxel/SharedVoxel/",
	}

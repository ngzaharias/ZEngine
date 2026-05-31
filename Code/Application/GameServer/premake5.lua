project "GameServer"
	kind "StaticLib"
	pchheader "GameServerPCH.h"
	pchsource "GameServer/GameServerPCH.cpp"
	location "%{wks.location}/Application/GameServer"

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
			"GameServer/**.h", 
			"GameServer/**.cpp", 
			"GameServer/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../Code/Application/GameServer/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Framework/Serialize/",
		"%{wks.location}/../Code/Modules/Cursor/ServerCursor/",
		"%{wks.location}/../Code/Modules/Cursor/SharedCursor/",
		"%{wks.location}/../Code/Modules/Hidden/ServerHidden/",
		"%{wks.location}/../Code/Modules/Hidden/SharedHidden/",
		"%{wks.location}/../Code/Modules/Inventory/SharedInventory/",
		"%{wks.location}/../Code/Modules/Network/ServerNetwork/",
		"%{wks.location}/../Code/Modules/Network/SharedNetwork/",
	}

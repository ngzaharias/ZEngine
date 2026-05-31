project "ServerCursor"
	kind "StaticLib"
	pchheader "CursorPCH.h"
	pchsource "ServerCursor/CursorPCH.cpp"
	location "%{wks.location}/Modules/Cursor/ServerCursor"

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
			"ServerCursor/**.h", 
			"ServerCursor/**.cpp", 
			"ServerCursor/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Framework/Render/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Modules/Cursor/ServerCursor/",
		"%{wks.location}/../Code/Modules/Cursor/SharedCursor/",
		"%{wks.location}/../Code/Modules/Network/ServerNetwork/",
	}
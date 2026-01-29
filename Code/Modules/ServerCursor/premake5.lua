project "ServerCursor"
	kind "StaticLib"
	pchheader "ServerCursorPCH.h"
	pchsource "ServerCursor/ServerCursorPCH.cpp"
	location "%{wks.location}/Modules/ServerCursor"
	
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
		"%{wks.location}/../Code/Modules/ServerCursor/",
		"%{wks.location}/../Code/Modules/SharedCursor/",
	}
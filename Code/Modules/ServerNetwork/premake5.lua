project "ServerNetwork"
	kind "StaticLib"
	pchheader "ServerNetworkPCH.h"
	pchsource "ServerNetwork/ServerNetworkPCH.cpp"
	location "%{wks.location}/Modules/ServerNetwork"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"ServerNetwork/**.h", 
			"ServerNetwork/**.cpp", 
			"ServerNetwork/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Modules/ServerNetwork/",
		"%{wks.location}/../Code/Modules/SharedNetwork/",
	}
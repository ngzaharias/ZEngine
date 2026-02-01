project "ClientNetwork"
	kind "StaticLib"
	pchheader "ClientNetworkPCH.h"
	pchsource "ClientNetwork/ClientNetworkPCH.cpp"
	location "%{wks.location}/Modules/ClientNetwork"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"ClientNetwork/**.h", 
			"ClientNetwork/**.cpp", 
			"ClientNetwork/**.inl" } },
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
		"%{wks.location}/../Code/Modules/ClientNetwork/",
		"%{wks.location}/../Code/Modules/SharedNetwork/",
	}
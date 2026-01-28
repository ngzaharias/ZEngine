project "SharedNetwork"
	kind "StaticLib"
	pchheader "SharedNetworkPCH.h"
	pchsource "SharedNetwork/SharedNetworkPCH.cpp"
	location "%{wks.location}/Modules/SharedNetwork"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"SharedNetwork/**.h", 
			"SharedNetwork/**.cpp", 
			"SharedNetwork/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Modules/SharedNetwork/",
	}
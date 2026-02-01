project "ServerHidden"
	kind "StaticLib"
	pchheader "ServerHiddenPCH.h"
	pchsource "ServerHidden/ServerHiddenPCH.cpp"
	location "%{wks.location}/Modules/ServerHidden"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"ServerHidden/**.h", 
			"ServerHidden/**.cpp", 
			"ServerHidden/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Modules/ServerHidden/",
		"%{wks.location}/../Code/Modules/SharedHidden/",
	}
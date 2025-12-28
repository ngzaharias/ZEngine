project "GameShared"
	kind "StaticLib"
	pchheader "GameSharedPCH.h"
	pchsource "GameShared/GameSharedPCH.cpp"
	location "%{wks.location}/Application/GameShared"

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"GameShared/**.h", 
			"GameShared/**.cpp", 
			"GameShared/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Application/GameShared/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Network/",
	}

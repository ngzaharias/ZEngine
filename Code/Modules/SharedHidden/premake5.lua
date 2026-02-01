project "SharedHidden"
	kind "StaticLib"
	pchheader "SharedHiddenPCH.h"
	pchsource "SharedHidden/SharedHiddenPCH.cpp"
	location "%{wks.location}/Modules/SharedHidden"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"SharedHidden/**.h", 
			"SharedHidden/**.cpp", 
			"SharedHidden/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Modules/SharedHidden/",
	}
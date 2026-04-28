project "NodeGraph_ut"
	kind "ConsoleApp"
	pchheader "NodeGraphPCH.h"
	pchsource "NodeGraph_ut/NodeGraphPCH.cpp"
	location "%{wks.location}/Editors/NodeGraph_ut"

	defines { "Z_UNIT_TEST" }

	disablewarnings 
	{ 
		"6237",
		"6319", 
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"NodeGraph_ut/**.h", 
			"NodeGraph_ut/**.cpp", 
			"NodeGraph_ut/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../Code/Editors/NodeGraph/",
		"%{wks.location}/../Code/Editors/NodeGraph_ut/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Serialize/",
	}
	
	links 
	{ 
		"Core",
		"Engine",
		"Math",
		"NodeGraph",
		"Serialize",
	}

	filter "files:NodeGraph_ut/Main.cpp"
		flags { "NoPCH" }
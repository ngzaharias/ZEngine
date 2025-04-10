project "Math_ut"
	kind "ConsoleApp"
	pchheader "MathPCH.h"
	pchsource "Math_ut/MathPCH.cpp"
	location "%{wks.location}/Math_ut"

	defines { "Z_UNIT_TEST" }

	disablewarnings 
	{ 
		"6237",
		"6319", 
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Math_ut/**.h", 
			"Math_ut/**.cpp", 
			"Math_ut/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/Math/",
	}
	
	links 
	{ 
		"Core",
		"Math",
	}

	filter "files:Math_ut/Main.cpp"
		flags { "NoPCH" }
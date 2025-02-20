project "Core_ut"
	kind "ConsoleApp"
	location "%{wks.location}/Core_ut"

	defines { "Z_UNIT_TEST" }

	disablewarnings 
	{ 
		"6237",
		"6319", 
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Core_ut/**.h", 
			"Core_ut/**.cpp", 
			"Core_ut/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/Math/",
	}

	links 
	{ 
		"Core", 
		"Math",
	}


project "Crafting_ut"
	kind "ConsoleApp"
	location "%{wks.location}/Modules/Crafting_ut"

	defines { "Z_UNIT_TEST" }

	disablewarnings 
	{ 
		"6237",
		"6319", 
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Crafting_ut/**.h", 
			"Crafting_ut/**.cpp", 
			"Crafting_ut/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Serialize/",
		"%{wks.location}/../Code/Modules/Crafting/",
	}

	libdirs 
	{ 
		"%{wks.location}/../3rdParty/optick/1.3.1/Library/",
	}

	links 
	{ 
		"Core", 
		"ECS", 
		"Engine", 
		"Math",
		"Serialize",

		"Crafting",

		"OptickCore.lib",
	}

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
	}

	filter "files:Crafting_ut/Main.cpp"
		flags { "NoPCH" }
project "SharedCrafting_ut"
	kind "ConsoleApp"
	location "%{wks.location}/Modules/Crafting/SharedCrafting_ut"

	defines { "Z_UNIT_TEST" }

	disablewarnings 
	{ 
		"6237",
		"6319", 
	}

	local root = path.getdirectory(_SCRIPT)
	files 
	{ 
		root .. "/premake5.lua",
		root .. "/*.natvis",
		root .. "/%{prj.name}/**.h",
		root .. "/%{prj.name}/**.cpp",
		root .. "/%{prj.name}/**.inl",
		root .. "/Resource/**",
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"SharedCrafting_ut/**.h", 
			"SharedCrafting_ut/**.cpp", 
			"SharedCrafting_ut/**.inl" } },
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
		"%{wks.location}/../Code/Modules/Crafting/SharedCrafting/",
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

		"SharedCrafting",

		"OptickCore.lib",
	}

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
	}

	filter "files:Crafting_ut/Main.cpp"
		flags { "NoPCH" }
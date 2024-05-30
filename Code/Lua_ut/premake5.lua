project "Lua_ut"
	kind "ConsoleApp"
	dependson { }
	location "%{wks.location}/Lua_ut"

	defines { "Z_UNIT_TEST" }

	disablewarnings 
	{ 
		"6237",
		"6319", 
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Lua_ut/**.h", 
			"Lua_ut/**.cpp", 
			"Lua_ut/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/lua/5.4.6/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
	}

	libdirs 
	{ 
		"%{wks.location}/../3rdParty/lua/5.4.6/Library/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Library/",
	}

	links 
	{ 
		"OptickCore.lib",
		"liblua54.lib",
	}

	postbuildcommands 
	{ 
		"{COPY} %{wks.location}/../3rdParty/lua/5.4.6/Binary/*.dll $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/optick/1.3.1/Binary/*.dll $(OutDir)",
	}
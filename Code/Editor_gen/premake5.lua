project "Editor_gen"
	kind "ConsoleApp"
	dependson { "Kodgen" }
	location "%{wks.location}/Editor_gen"

	debugargs { 
		"%{wks.location}/../Code/Editor/Editor/",
		"%{wks.location}/Generated/Editor/Editor/",	}

	vpaths { 
		{ ["Source/*"] = {  
			"Editor_gen/**.h", 
			"Editor_gen/**.cpp", 
			"Editor_gen/**.inl" } }, }

	includedirs { 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/clang-c/Include/",
		"%{wks.location}/../3rdParty/toml11/",
		"%{wks.location}/../Code/Editor_gen/",
		"%{wks.location}/../Code/Kodgen/", }

	libdirs {
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/clang-c/Library/",
		"%{wks.location}/Build/Kodgen/%{cfg.buildcfg}_%{cfg.platform}/", }

	links { 
		"clang.lib",
		"Kodgen.lib", }

	postbuildcommands { 
		"{COPY} %{wks.location}/../3rdParty/vswhere.exe $(OutDir)",
		"{COPY} %{wks.location}/../3rdParty/clang-c/Binary/*.dll $(OutDir)", }
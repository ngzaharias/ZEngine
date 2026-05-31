project "Network"
	kind "StaticLib"
	location "%{wks.location}/Framework/Network"

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
			"Network/**.h", 
			"Network/**.cpp", 
			"Network/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/Network/",
	}

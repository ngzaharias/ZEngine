project "SharedNetwork"
	kind "StaticLib"
	pchheader "NetworkPCH.h"
	pchsource "SharedNetwork/NetworkPCH.cpp"
	location "%{wks.location}/Modules/Network/SharedNetwork"

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
			"SharedNetwork/**.h", 
			"SharedNetwork/**.cpp", 
			"SharedNetwork/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Modules/Network/SharedNetwork/",
	}
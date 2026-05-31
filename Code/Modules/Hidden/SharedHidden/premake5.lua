project "SharedHidden"
	kind "StaticLib"
	pchheader "HiddenPCH.h"
	pchsource "SharedHidden/HiddenPCH.cpp"
	location "%{wks.location}/Modules/Hidden/SharedHidden"

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
		"%{wks.location}/../Code/Modules/Hidden/SharedHidden/",
	}
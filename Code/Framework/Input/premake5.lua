project "Input"
	kind "StaticLib"
	location "%{wks.location}/Framework/Input"

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
			"Input/**.h", 
			"Input/**.cpp", 
			"Input/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
	}

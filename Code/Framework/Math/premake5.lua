project "Math"
	kind "StaticLib"
	location "%{wks.location}/Framework/Math"

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
			"Math/**.h", 
			"Math/**.cpp", 
			"Math/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../Code/Framework/Math/",
	}
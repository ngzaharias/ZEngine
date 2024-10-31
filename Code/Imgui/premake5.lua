project "Imgui"
	kind "StaticLib"
	location "%{wks.location}/Imgui"

	files 
	{ 
		"%{prj.location}/**.h",
		"%{prj.location}/**.inl",
		"%{prj.location}/**.cpp",

		"%{wks.location}/../3rdParty/imgui/1.91/Include/imgui/*.h",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/imgui/*.inl",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/imgui/*.cpp",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/imnodes/*.h",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/imnodes/*.inl",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/imnodes/*.cpp",
		"%{wks.location}/../3rdParty/imguizmo/1.83/Include/imguizmo/*.h",
		"%{wks.location}/../3rdParty/imguizmo/1.83/Include/imguizmo/*.inl",
		"%{wks.location}/../3rdParty/imguizmo/1.83/Include/imguizmo/*.cpp",
	}
		
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Imgui/**.h", 
			"Imgui/**.inl",
			"Imgui/**.cpp", 
			} },
		{ ["imgui/*"] = {  
			"../../3rdParty/imgui/1.91/Include/imgui/*.h",
			"../../3rdParty/imgui/1.91/Include/imgui/*.inl",
			"../../3rdParty/imgui/1.91/Include/imgui/*.cpp",
			} },
		{ ["imguizmo/*"] = {  
			"../../3rdParty/imguizmo/1.83/Include/imguizmo/*.h",
			"../../3rdParty/imguizmo/1.83/Include/imguizmo/*.inl",
			"../../3rdParty/imguizmo/1.83/Include/imguizmo/*.cpp",
			} },
		{ ["imnodes/*"] = {  
			"../../3rdParty/imnodes/0.5/Include/imnodes/*.h",
			"../../3rdParty/imnodes/0.5/Include/imnodes/*.inl",
			"../../3rdParty/imnodes/0.5/Include/imnodes/*.cpp",
			} },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/imguizmo/1.83/Include/",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/SFML/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/../Code/Math/",
	}

	links 
	{ 
		"Core", 
		"Math",
	}


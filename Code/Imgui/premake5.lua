project "Imgui"
	kind "StaticLib"
	dependson { "Core", "Math" }
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
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/",
		"%{wks.location}/../3rdParty/ImGuizmo/",
		"%{wks.location}/../3rdParty/SFML/2.6.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/../Code/Math/",
	}

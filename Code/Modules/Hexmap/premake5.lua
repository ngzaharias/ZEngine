project "Hexmap"
	kind "StaticLib"
	pchheader "HexmapPCH.h"
	pchsource "Hexmap/HexmapPCH.cpp"
	location "%{wks.location}/Modules/Hexmap"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Hexmap/**.h", 
			"Hexmap/**.cpp", 
			"Hexmap/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/glew/2.1.0/Include/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Render/",
		"%{wks.location}/../Code/Modules/Hexmap/",
	}
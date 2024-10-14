project "Engine"
	kind "StaticLib"
	pchheader "EnginePCH.h"
	pchsource "Engine/EnginePCH.cpp"
	location "%{wks.location}/Engine"

	removefiles 
	{ 
		"Engine/Vulkan/*", 
	}

	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Engine/**.h", 
			"Engine/**.cpp", 
			"Engine/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/assimp/5.2.4/Include/",
		"%{wks.location}/../3rdParty/Freetype/2.13.3/Include/",
		"%{wks.location}/../3rdParty/glew/2.1.0/Include/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/imgui/1.87/Include/",
		"%{wks.location}/../3rdParty/imguizmo/1.83/Include/",
		"%{wks.location}/../3rdParty/imnodes/0.5/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../3rdParty/PhysX/Include/",
		"%{wks.location}/../3rdParty/SFML/Include/",
		"%{wks.location}/../3rdParty/SteamworksSDK/Include/",
		"%{wks.location}/../3rdParty/yojimbo/1.2.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/../Code/Math/",
		"%{wks.location}/../Code/Network/",
		"%{wks.location}/../Code/SteamBinding/",
	}

	links 
	{ 
		"Core", 
		"Imgui", 
		"Math", 
		"Network", 
		"SteamBinding",
	}
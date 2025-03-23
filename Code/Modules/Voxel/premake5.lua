project "Voxel"
	kind "StaticLib"
	pchheader "VoxelPCH.h"
	pchsource "Voxel/VoxelPCH.cpp"
	location "%{wks.location}/Modules/Voxel"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"Voxel/**.h", 
			"Voxel/**.cpp", 
			"Voxel/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/glew/2.1.0/Include/",
		"%{wks.location}/../3rdParty/glfw/3.3.4/Include/",
		"%{wks.location}/../3rdParty/imgui/1.91/Include/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Core/",
		"%{wks.location}/../Code/ECS/",
		"%{wks.location}/../Code/Engine/",
		"%{wks.location}/../Code/Imgui/",
		"%{wks.location}/../Code/Input/",
		"%{wks.location}/../Code/Math/",
		"%{wks.location}/../Code/Modules/Voxel/",
	}

	links 
	{ 
		"Core", 
		"ECS", 
		"Engine", 
		"Imgui", 
		"Math", 
	}
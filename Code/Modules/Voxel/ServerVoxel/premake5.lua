project "ServerVoxel"
	kind "StaticLib"
	pchheader "VoxelPCH.h"
	pchsource "ServerVoxel/VoxelPCH.cpp"
	location "%{wks.location}/Modules/Voxel/ServerVoxel"

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
			"ServerVoxel/**.h", 
			"ServerVoxel/**.cpp", 
			"ServerVoxel/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Serialize/",
		"%{wks.location}/../Code/Modules/Voxel/ServerVoxel/",
		"%{wks.location}/../Code/Modules/Voxel/SharedVoxel/",
	}
project "ServerReplication"
	kind "StaticLib"
	pchheader "ReplicationPCH.h"
	pchsource "ServerReplication/ReplicationPCH.cpp"
	location "%{wks.location}/Modules/Replication/ServerReplication"

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
			"ServerReplication/**.h", 
			"ServerReplication/**.cpp", 
			"ServerReplication/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Network/",
		"%{wks.location}/../Code/Modules/Cursor/SharedCursor/",
		"%{wks.location}/../Code/Modules/Replication/ServerReplication/",
		"%{wks.location}/../Code/Modules/Voxel/SharedVoxel/",
	}
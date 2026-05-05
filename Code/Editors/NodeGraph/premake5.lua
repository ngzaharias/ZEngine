project "NodeGraph"
	kind "StaticLib"
	location "%{wks.location}/Editors/NodeGraph"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"NodeGraph/**.h", 
			"NodeGraph/**.cpp", 
			"NodeGraph/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/magic_enum/0.8.0/Include/",
		"%{wks.location}/../Code/Editors/NodeGraph/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Imgui/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Framework/Serialize/",
	}
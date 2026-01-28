project "SharedCursor"
	kind "StaticLib"
	location "%{wks.location}/Modules/SharedCursor"
	
	vpaths 
	{ 
		{ ["Source/*"] = {  
			"SharedCursor/**.h", 
			"SharedCursor/**.cpp", 
			"SharedCursor/**.inl" } },
	}

	includedirs 
	{ 
		"%{wks.location}/../3rdParty/",
		"%{wks.location}/../3rdParty/optick/1.3.1/Include/",
		"%{wks.location}/../Code/Framework/Core/",
		"%{wks.location}/../Code/Framework/ECS/",
		"%{wks.location}/../Code/Framework/Engine/",
		"%{wks.location}/../Code/Framework/Input/",
		"%{wks.location}/../Code/Framework/Render/",
		"%{wks.location}/../Code/Framework/Math/",
		"%{wks.location}/../Code/Modules/SharedCursor/",
	}
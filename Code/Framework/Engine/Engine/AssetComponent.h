#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng
{
	struct Asset;
}

namespace eng
{
	// \brief 
	struct AssetComponent final : public ecs::Component
	{
		Set<str::Guid> m_Assets = {};
		Map<str::Guid, eng::Asset*> m_Loaded = {};
	};
}
#pragma once

#include <Core/String.h>

namespace ecs
{
	using ComponentId = int32;

	struct ComponentEntry
	{
		ecs::ComponentId m_ComponentId = INT32_MAX;
		str::String m_DebugName = { };
	};
}
#pragma once

#include "Core/String.h"

namespace ecs
{
	using ComponentId = int32;

	struct ComponentEntry
	{
		str::String m_Name = { };
	};
}
#pragma once

#include "Core/String.h"
#include "ECS/Event.h"

namespace ecs
{
	struct DebugEvent final : public ecs::Event<DebugEvent>
	{
		str::String m_Data = {};
	};
}
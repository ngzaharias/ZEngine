#pragma once

#include "Core/String.h"
#include "ECS/Event.h"
#include "ECS/IsReplicated.h"

namespace ecs
{
	struct DebugEvent final : public ecs::Event<DebugEvent>, ecs::IsReplicated
	{
		str::String m_Data = {};
	};
}
#pragma once

#include "Core/String.h"
#include "ECS/Event.h"
#include "ECS/IsReplicated.h"

namespace debug
{
	struct NetworkEvent final : public ecs::Event<NetworkEvent>, ecs::IsReplicated
	{
		str::String m_Data = {};
	};
}
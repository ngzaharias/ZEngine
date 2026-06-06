#pragma once

#include "ECS/Component.h"
#include "ECS/IsReplicated.h"

namespace shared::tilemap
{
	struct AgentComponent final : public ecs::Component
		, ecs::IsReplicated
	{
	};
}
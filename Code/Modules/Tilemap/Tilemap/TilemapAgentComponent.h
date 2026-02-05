#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace tilemap
{
	struct AgentComponent final : public ecs::Component<AgentComponent>
		, ecs::IsPrototype
	{
	};
}
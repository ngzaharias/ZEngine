#pragma once

#include "ECS/Component.h"

namespace tilemap
{
	struct AgentComponent final : public ecs::Component<AgentComponent>
	{
	};
}
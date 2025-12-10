#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct EntityWindowRequest final : public ecs::Event<EntityWindowRequest> { };
}
#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct EntityWindowRequest final : public ecs::Event<EntityWindowRequest> { };
}
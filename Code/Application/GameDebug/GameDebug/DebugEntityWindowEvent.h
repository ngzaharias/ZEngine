#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct EntityWindowEvent final : public ecs::Event<EntityWindowEvent> { };
}
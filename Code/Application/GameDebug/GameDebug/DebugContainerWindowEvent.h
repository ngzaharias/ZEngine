#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct ContainerWindowEvent final : public ecs::Event<ContainerWindowEvent> { };
}
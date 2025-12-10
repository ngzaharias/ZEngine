#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct ContainerWindowRequest final : public ecs::Event<ContainerWindowRequest> { };
}
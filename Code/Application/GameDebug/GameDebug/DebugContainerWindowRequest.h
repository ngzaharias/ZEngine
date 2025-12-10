#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct ContainerWindowRequest final : public ecs::Event<ContainerWindowRequest> { };
}
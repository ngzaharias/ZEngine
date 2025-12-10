#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct NetworkWindowRequest final : public ecs::Event<NetworkWindowRequest> { };
}
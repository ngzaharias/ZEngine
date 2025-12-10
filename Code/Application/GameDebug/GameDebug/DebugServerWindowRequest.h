#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct ServerWindowRequest final : public ecs::Event<ServerWindowRequest> { };
}
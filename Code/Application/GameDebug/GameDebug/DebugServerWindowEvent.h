#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct ServerWindowEvent final : public ecs::Event<ServerWindowEvent> { };
}
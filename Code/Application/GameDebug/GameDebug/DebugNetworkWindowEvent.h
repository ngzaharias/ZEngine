#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct NetworkWindowEvent final : public ecs::Event<NetworkWindowEvent> { };
}
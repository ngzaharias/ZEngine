#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct ClientWindowEvent final : public ecs::Event<ClientWindowEvent> { };
}
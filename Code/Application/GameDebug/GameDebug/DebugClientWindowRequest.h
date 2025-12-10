#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct ClientWindowRequest final : public ecs::Event<ClientWindowRequest> { };
}
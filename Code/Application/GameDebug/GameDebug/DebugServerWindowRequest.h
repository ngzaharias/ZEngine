#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct ServerWindowRequest final : public ecs::Event<ServerWindowRequest> { };
}
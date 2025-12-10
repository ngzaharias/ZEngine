#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct NetworkWindowRequest final : public ecs::Event<NetworkWindowRequest> { };
}
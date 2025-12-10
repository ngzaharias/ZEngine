#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct ClientWindowRequest final : public ecs::Event<ClientWindowRequest> { };
}
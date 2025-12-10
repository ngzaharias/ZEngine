#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct BufferWindowRequest final : public ecs::Event<BufferWindowRequest> { };
}
#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct BufferWindowRequest final : public ecs::Event<BufferWindowRequest> { };
}
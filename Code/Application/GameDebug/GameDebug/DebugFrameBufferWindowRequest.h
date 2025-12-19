#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct FrameBufferWindowRequest final : public ecs::Event<FrameBufferWindowRequest> { };
}
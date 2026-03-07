#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct FrameBufferWindowEvent final : public ecs::Event<FrameBufferWindowEvent> { };
}
#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct SplineWindowEvent final : public ecs::Event<SplineWindowEvent> { };
}
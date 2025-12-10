#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct SplineWindowRequest final : public ecs::Event<SplineWindowRequest> { };
}
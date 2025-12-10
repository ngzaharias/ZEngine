#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct SplineWindowRequest final : public ecs::Event<SplineWindowRequest> { };
}
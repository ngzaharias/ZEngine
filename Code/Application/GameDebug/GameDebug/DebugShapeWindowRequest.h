#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct ShapeWindowRequest final : public ecs::Event<ShapeWindowRequest> { };
}
#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct ShapeWindowRequest final : public ecs::Event<ShapeWindowRequest> { };
}
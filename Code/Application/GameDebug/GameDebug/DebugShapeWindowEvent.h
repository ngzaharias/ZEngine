#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct ShapeWindowEvent final : public ecs::Event<ShapeWindowEvent> { };
}
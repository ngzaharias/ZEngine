#pragma once

#include "ECS/Event.h"

namespace debug::level
{
	struct OpenEvent final : public ecs::Event<OpenEvent> { };
}
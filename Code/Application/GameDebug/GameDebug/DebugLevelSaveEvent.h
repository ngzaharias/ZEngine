#pragma once

#include "ECS/Event.h"

namespace debug::level
{
	struct SaveEvent final : public ecs::Event<SaveEvent> { };
}
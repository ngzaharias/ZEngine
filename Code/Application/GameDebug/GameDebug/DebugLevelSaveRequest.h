#pragma once

#include "ECS/Event.h"

namespace debug::level
{
	struct SaveRequest final : public ecs::Event<SaveRequest> { };
}
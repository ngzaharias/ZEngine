#pragma once

#include "ECS/Event.h"

namespace debug::level
{
	struct SaveAsRequest final : public ecs::Event<SaveAsRequest> { };
}
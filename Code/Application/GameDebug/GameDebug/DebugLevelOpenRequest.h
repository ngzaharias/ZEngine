#pragma once

#include "ECS/Event.h"

namespace debug::level
{
	struct OpenRequest final : public ecs::Event<OpenRequest> { };
}
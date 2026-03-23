#pragma once

#include "ECS/Event.h"

namespace debug::level
{
	struct ReloadEvent final : public ecs::Event { };
}
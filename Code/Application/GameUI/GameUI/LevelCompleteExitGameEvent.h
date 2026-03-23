#pragma once

#include "ECS/Event.h"

namespace gui::level_complete
{
	struct ExitGameEvent final : public ecs::Event { };
}
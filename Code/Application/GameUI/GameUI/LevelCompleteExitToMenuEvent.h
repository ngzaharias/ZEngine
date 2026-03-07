#pragma once

#include "ECS/Event.h"

namespace gui::level_complete
{
	struct ExitToMenuEvent final : public ecs::Event<ExitToMenuEvent> { };
}
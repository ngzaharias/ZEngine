#pragma once

#include "ECS/Event.h"

namespace gui::game_menu
{
	struct ExitToMenuEvent final : public ecs::Event<ExitToMenuEvent> { };
}
#pragma once

#include "ECS/Event.h"

namespace gui::game_menu
{
	struct ExitGameEvent final : public ecs::Event<ExitGameEvent> { };
}
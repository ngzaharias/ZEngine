#pragma once

#include "ECS/Event.h"

namespace gui::game_menu
{
	struct ExitGameRequest final : public ecs::Event<ExitGameRequest> { };
}
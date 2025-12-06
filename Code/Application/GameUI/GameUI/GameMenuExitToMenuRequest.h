#pragma once

#include "ECS/Event.h"

namespace gui::game_menu
{
	struct ExitToMenuRequest final : public ecs::Event<ExitToMenuRequest> { };
}
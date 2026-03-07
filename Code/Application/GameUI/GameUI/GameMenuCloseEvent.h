#pragma once

#include "ECS/Event.h"

namespace gui::game_menu
{
	struct CloseEvent final : public ecs::Event<CloseEvent> { };
}
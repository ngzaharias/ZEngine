#pragma once

#include "ECS/Event.h"

namespace gui::game_menu
{
	struct CloseRequestEvent final : public ecs::Event<CloseRequestEvent> { };
}
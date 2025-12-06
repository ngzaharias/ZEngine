#pragma once

#include "ECS/Event.h"

namespace gui::game_menu
{
	struct CloseRequest final : public ecs::Event<CloseRequest> { };
}
#pragma once

#include "ECS/Event.h"

namespace gui::game_menu
{
	struct OpenRequest final : public ecs::Event<OpenRequest> { };
}
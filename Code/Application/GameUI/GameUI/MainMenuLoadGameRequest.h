#pragma once

#include "ECS/Event.h"

namespace gui::main_menu
{
	struct LoadGameRequest final : public ecs::Event<LoadGameRequest> { };
}

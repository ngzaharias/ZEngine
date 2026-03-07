#pragma once

#include "ECS/Event.h"

namespace gui::main_menu
{
	struct ExitGameEvent final : public ecs::Event<ExitGameEvent> { };
}

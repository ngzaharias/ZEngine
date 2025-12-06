#pragma once

#include "ECS/Event.h"

namespace gui::main_menu
{
	struct ExitGameRequest final : public ecs::Event<ExitGameRequest> { };
}

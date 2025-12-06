#pragma once

#include "ECS/Event.h"

namespace gui::main_menu
{
	struct ContinueGameRequest final : public ecs::Event<ContinueGameRequest> { };
}

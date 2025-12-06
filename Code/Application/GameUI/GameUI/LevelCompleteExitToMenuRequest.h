#pragma once

#include "ECS/Event.h"

namespace gui::level_complete
{
	struct ExitToMenuRequest final : public ecs::Event<ExitToMenuRequest> { };
}
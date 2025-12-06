#pragma once

#include "ECS/Event.h"

namespace gui::level_complete
{
	struct ExitGameRequest final : public ecs::Event<ExitGameRequest> { };
}
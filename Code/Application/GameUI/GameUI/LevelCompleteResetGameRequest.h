#pragma once

#include "ECS/Event.h"

namespace gui::level_complete
{
	struct ResetGameRequest final : public ecs::Event<ResetGameRequest> { };
}
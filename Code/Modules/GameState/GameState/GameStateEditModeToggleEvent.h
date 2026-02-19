#pragma once

#include "ECS/Event.h"

namespace gamestate
{
	struct EditModeToggleEvent final : public ecs::Event<EditModeToggleEvent> { };
};
#pragma once

#include "ECS/Event.h"

namespace shared::gamestate
{
	struct EditModeToggleEvent final : public ecs::Event { };
};
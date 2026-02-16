#pragma once

#include "ECS/Event.h"

namespace settings
{
	struct EditorToggleEvent final : public ecs::Event<EditorToggleEvent> { };
};
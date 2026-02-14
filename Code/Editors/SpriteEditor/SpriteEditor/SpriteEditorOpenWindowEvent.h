#pragma once

#include "ECS/Event.h"

namespace editor::sprite
{
	struct OpenWindowEvent final : public ecs::Event<OpenWindowEvent> { };
}
#pragma once

#include "ECS/Event.h"

namespace editor::sprite
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}
#pragma once

#include "ECS/Event.h"

namespace editor::trajectory
{
	struct WindowEvent final : public ecs::Event<WindowEvent> { };
}
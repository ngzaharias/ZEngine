#pragma once

#include "ECS/Event.h"

namespace debug::inventory
{
	struct WindowEvent final : public ecs::Event<WindowEvent> { };
}
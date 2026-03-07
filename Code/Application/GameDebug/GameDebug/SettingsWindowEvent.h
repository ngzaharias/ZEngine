#pragma once

#include "ECS/Event.h"

namespace debug::settings
{
	struct WindowEvent final : public ecs::Event<WindowEvent> { };
}
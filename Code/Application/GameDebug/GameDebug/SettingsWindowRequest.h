#pragma once

#include "ECS/Event.h"

namespace debug::settings
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}
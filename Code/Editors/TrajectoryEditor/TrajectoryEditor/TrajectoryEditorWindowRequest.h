#pragma once

#include "ECS/Event.h"

namespace editor::trajectory
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}
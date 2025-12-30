#pragma once

#include "ECS/Event.h"

namespace editor::input
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}
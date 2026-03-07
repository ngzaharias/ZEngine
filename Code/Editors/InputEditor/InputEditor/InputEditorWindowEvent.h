#pragma once

#include "ECS/Event.h"

namespace editor::input
{
	struct WindowEvent final : public ecs::Event<WindowEvent> { };
}
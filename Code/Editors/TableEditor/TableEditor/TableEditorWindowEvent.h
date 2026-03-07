#pragma once

#include "ECS/Event.h"

namespace editor::table
{
	struct WindowEvent final : public ecs::Event<WindowEvent> { };
}
#pragma once

#include "ECS/Event.h"

namespace editor::theme
{
	struct WindowEvent final : public ecs::Event<WindowEvent> { };
}
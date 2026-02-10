#pragma once

#include "ECS/Event.h"

namespace editor::theme
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}
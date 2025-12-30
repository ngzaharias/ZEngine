#pragma once

#include "ECS/Event.h"

namespace editor::table
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}
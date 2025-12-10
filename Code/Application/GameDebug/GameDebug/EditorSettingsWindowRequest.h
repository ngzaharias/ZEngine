#pragma once

#include "ECS/Event.h"

namespace editor::settings
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}
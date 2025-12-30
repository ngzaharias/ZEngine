#pragma once

#include "ECS/Event.h"

namespace editor::flipbook
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}
#pragma once

#include "ECS/Event.h"

namespace editor::texture
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}
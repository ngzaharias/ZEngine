#pragma once

#include "ECS/Event.h"

namespace editor::texture
{
	struct OpenWindowEvent final : public ecs::Event<OpenWindowEvent> { };
}
#pragma once

#include "ECS/Event.h"

namespace editor::inspector
{
	struct OpenWindowEvent final : public ecs::Event<OpenWindowEvent> { };
}
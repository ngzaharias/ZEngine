#pragma once

#include "ECS/Event.h"

namespace editor::outliner
{
	struct OpenWindowEvent final : public ecs::Event<OpenWindowEvent> { };
}
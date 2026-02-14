#pragma once

#include "ECS/Event.h"

namespace editor::flipbook
{
	struct OpenWindowEvent final : public ecs::Event<OpenWindowEvent> { };
}
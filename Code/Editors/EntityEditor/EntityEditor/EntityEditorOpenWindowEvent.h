#pragma once

#include "ECS/Event.h"

namespace editor::entity
{
	struct OpenWindowEvent final : public ecs::Event<OpenWindowEvent> { };
}
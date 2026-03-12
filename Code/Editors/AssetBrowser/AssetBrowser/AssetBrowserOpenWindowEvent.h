#pragma once

#include "ECS/Event.h"

namespace editor::assets
{
	struct OpenWindowEvent final : public ecs::Event<OpenWindowEvent> { };
}
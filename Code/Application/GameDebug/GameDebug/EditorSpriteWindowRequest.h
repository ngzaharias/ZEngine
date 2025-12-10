#pragma once

#include "ECS/Event.h"

namespace editor
{
	struct SpriteWindowRequest final : public ecs::Event<SpriteWindowRequest> { };
}
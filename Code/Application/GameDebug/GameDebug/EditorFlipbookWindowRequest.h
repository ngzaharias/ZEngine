#pragma once

#include "ECS/Event.h"

namespace editor
{
	struct FlipbookWindowRequest final : public ecs::Event<FlipbookWindowRequest> { };
}
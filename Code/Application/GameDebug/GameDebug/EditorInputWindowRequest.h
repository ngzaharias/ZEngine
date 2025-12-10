#pragma once

#include "ECS/Event.h"

namespace editor
{
	struct InputWindowRequest final : public ecs::Event<InputWindowRequest> { };
}
#pragma once

#include "ECS/Event.h"

namespace editor
{
	struct EntityWindowRequest final : public ecs::Event<EntityWindowRequest> { };
}
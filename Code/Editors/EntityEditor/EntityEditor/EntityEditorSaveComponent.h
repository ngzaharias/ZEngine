#pragma once

#include "ECS/Component.h"

namespace editor::entity
{
	struct SaveComponent final : public ecs::Component<SaveComponent> { };
}

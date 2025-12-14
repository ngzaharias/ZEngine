#pragma once

#include "ECS/Component.h"

namespace editor
{
	struct EntitySaveComponent final : public ecs::Component<EntitySaveComponent> { };
}

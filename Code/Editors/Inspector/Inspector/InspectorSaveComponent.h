#pragma once

#include "ECS/Component.h"

namespace editor::inspector
{
	struct SaveComponent final : public ecs::Component<SaveComponent> { };
}

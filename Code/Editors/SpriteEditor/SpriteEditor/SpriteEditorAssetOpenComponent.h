#pragma once

#include "ECS/Component.h"

namespace editor::sprite
{
	struct AssetOpenComponent final : public ecs::Component<AssetOpenComponent> { };
}

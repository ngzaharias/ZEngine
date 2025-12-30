#pragma once

#include "ECS/Component.h"

namespace editor::texture
{
	struct AssetOpenComponent final : public ecs::Component<AssetOpenComponent> { };
}

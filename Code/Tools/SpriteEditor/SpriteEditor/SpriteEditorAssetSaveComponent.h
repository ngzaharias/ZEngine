#pragma once

#include "ECS/Component.h"

namespace editor::sprite
{
	struct AssetSaveComponent final : public ecs::Component<AssetSaveComponent> { };
}

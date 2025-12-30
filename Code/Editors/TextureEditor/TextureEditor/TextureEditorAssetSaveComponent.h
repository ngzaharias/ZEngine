#pragma once

#include "ECS/Component.h"

namespace editor::texture
{
	struct AssetSaveComponent final : public ecs::Component<AssetSaveComponent> { };
}

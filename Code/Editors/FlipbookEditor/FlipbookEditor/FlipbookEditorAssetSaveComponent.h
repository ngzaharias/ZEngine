#pragma once

#include "ECS/Component.h"

namespace editor::flipbook
{
	struct AssetSaveComponent final : public ecs::Component<AssetSaveComponent> { };
}

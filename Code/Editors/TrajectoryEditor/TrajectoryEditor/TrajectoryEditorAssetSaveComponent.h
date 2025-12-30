#pragma once

#include "ECS/Component.h"

namespace editor::trajectory
{
	struct AssetSaveComponent final : public ecs::Component<AssetSaveComponent> { };
};
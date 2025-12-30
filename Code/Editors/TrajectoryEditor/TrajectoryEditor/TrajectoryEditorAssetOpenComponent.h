#pragma once

#include "ECS/Component.h"

namespace editor::trajectory
{
	struct AssetOpenComponent final : public ecs::Component<AssetOpenComponent> { };
};
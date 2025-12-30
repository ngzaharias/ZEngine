#pragma once

#include "ECS/Component.h"

namespace editor::flipbook
{
	struct AssetOpenComponent final : public ecs::Component<AssetOpenComponent> { };
}

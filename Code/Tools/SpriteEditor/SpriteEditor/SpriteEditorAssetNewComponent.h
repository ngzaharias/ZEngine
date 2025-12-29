#pragma once

#include "ECS/Component.h"

namespace editor::sprite
{
	struct AssetNewComponent final : public ecs::Component<AssetNewComponent> { };
}

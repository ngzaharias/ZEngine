#pragma once

#include "ECS/Component.h"

namespace editor::texture
{
	struct AssetNewComponent final : public ecs::Component<AssetNewComponent> { };
}

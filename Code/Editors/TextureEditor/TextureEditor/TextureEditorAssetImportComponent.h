#pragma once

#include "ECS/Component.h"

namespace editor::texture
{
	struct AssetImportComponent final : public ecs::Component<AssetImportComponent> { };
}

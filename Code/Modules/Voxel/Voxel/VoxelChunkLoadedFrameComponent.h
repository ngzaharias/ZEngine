#pragma once

#include "ECS/Component.h"

namespace voxel
{
	struct ChunkLoadedFrameComponent final : public ecs::Component<ChunkLoadedFrameComponent> { };
}
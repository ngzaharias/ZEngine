#pragma once

#include "ECS/Component.h"

namespace voxel
{
	struct ChunkChangedFrameComponent final : public ecs::Component<ChunkChangedFrameComponent> { };
}
#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "SharedVoxel/VoxelTypes.h"

namespace voxel
{
	struct ChunkComponent final : public ecs::Component
	{
		Array<voxel::Block> m_Data;
	};
}
#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "SharedVoxel/VoxelTypes.h"

namespace shared::voxel
{
	struct ChunkComponent final : public ecs::Component
	{
		Array<shared::voxel::Block> m_Data;
	};
}
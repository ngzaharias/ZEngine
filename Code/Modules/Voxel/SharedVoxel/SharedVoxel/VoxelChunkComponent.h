#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "ECS/IsReplicated.h"
#include "SharedVoxel/VoxelTypes.h"

namespace shared::voxel
{
	struct ChunkComponent final : public ecs::Component
		, ecs::IsReplicated
	{
		Array<shared::voxel::Block> m_Data;
	};
}
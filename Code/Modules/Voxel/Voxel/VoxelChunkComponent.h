#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Voxel/VoxelTypes.h"

namespace voxel
{
	struct ChunkComponent final : public ecs::Component<ChunkComponent>
	{
		Array<voxel::Block> m_Data;
	};
}
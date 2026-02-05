#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Voxel/VoxelTypes.h"

namespace voxel
{
	struct ChunkComponent final : public ecs::Component<ChunkComponent>
		, ecs::IsPrototype
	{
		Array<voxel::Block> m_Data;
	};
}
#pragma once

#include <ECS/Component.h>

#include <Engine/VoxelTypes.h>

namespace voxel
{
	struct ChunkComponent : public ecs::Component<ChunkComponent>
	{
		Array<voxel::Block> m_Data;
	};

	struct ChunkChangedEventComponent : public ecs::Component<ChunkChangedEventComponent>
	{
	};

	struct ChunkLoadedEventComponent : public ecs::Component<ChunkLoadedEventComponent>
	{
	};
}
#pragma once

#include <Core/Vector.h>

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

	struct Modify { Vector3f m_WorldPos; voxel::Block m_Data; };
	struct ModifyComponent : public ecs::Component<ModifyComponent>
	{
		Array<Modify> m_Changes;
	};
}
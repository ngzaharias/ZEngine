#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Voxel/VoxelTypes.h"
#include "Math/Vector.h"

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

	struct ModifySettingsComponent : public ecs::SingletonComponent<ModifySettingsComponent>
	{
		voxel::EType m_Type = voxel::EType::None;
		int32 m_Radius = 0;
	};
}
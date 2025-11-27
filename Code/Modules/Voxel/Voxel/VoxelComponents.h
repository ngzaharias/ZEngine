#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "ECS/Singleton.h"
#include "Math/Vector.h"
#include "Voxel/VoxelTypes.h"

namespace voxel
{
	struct ChunkComponent final : public ecs::Component<ChunkComponent>
	{
		Array<voxel::Block> m_Data;
	};

	struct ChunkChangedEventComponent final : public ecs::Component<ChunkChangedEventComponent>
	{
	};

	struct ChunkLoadedEventComponent final : public ecs::Component<ChunkLoadedEventComponent>
	{
	};

	struct Modify { Vector3f m_WorldPos; voxel::Block m_Data; };
	struct ModifyComponent : public ecs::Component<ModifyComponent>
	{
		Array<Modify> m_Changes;
	};

	struct ModifySettingsSingleton final : public ecs::Singleton<ModifySettingsSingleton>
	{
		voxel::EType m_Type = voxel::EType::None;
		int32 m_Radius = 0;
	};
}
#pragma once

#include "ECS/Singleton.h"
#include "Voxel/VoxelTypes.h"

namespace voxel
{
	struct ModifySettingsSingleton final : public ecs::Singleton
	{
		voxel::EType m_Type = voxel::EType::None;
		int32 m_Radius = 0;
	};
}
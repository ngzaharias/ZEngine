#pragma once

#include "ECS/Component.h"
#include "SharedVoxel/VoxelTypes.h"

namespace voxel
{
	struct ModifySettingsComponent final : public ecs::StaticComponent
	{
		voxel::EType m_Type = voxel::EType::None;
		int32 m_Radius = 0;
	};
}
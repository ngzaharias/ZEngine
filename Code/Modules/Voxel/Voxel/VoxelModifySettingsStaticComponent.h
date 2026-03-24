#pragma once

#include "ECS/Component.h"
#include "Voxel/VoxelTypes.h"

namespace voxel
{
	struct ModifySettingsStaticComponent final : public ecs::StaticComponent
	{
		voxel::EType m_Type = voxel::EType::None;
		int32 m_Radius = 0;
	};
}
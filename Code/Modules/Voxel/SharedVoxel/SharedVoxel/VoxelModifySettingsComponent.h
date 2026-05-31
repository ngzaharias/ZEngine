#pragma once

#include "ECS/Component.h"
#include "SharedVoxel/VoxelTypes.h"

namespace shared::voxel
{
	struct ModifySettingsComponent final : public ecs::StaticComponent
	{
		EType m_Type = EType::None;
		int32 m_Radius = 0;
	};
}
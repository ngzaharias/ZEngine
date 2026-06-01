#pragma once

#include "ECS/Component.h"
#include "SharedVoxel/VoxelTypes.h"

namespace client::voxel
{
	struct ModifySettingsComponent final : public ecs::StaticComponent
	{
		shared::voxel::EType m_Type = shared::voxel::EType::None;
		int32 m_Radius = 0;
	};
}
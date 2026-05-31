#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Math/Vector.h"
#include "SharedVoxel/VoxelTypes.h"

namespace shared::voxel
{
	struct Modify { Vector3f m_WorldPos; shared::voxel::Block m_Data; };
	struct ModifyComponent final : public ecs::Component
	{
		Array<Modify> m_Changes;
	};
}
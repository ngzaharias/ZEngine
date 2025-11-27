#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Math/Vector.h"
#include "Voxel/VoxelTypes.h"

namespace voxel
{
	struct Modify { Vector3f m_WorldPos; voxel::Block m_Data; };
	struct ModifyComponent : public ecs::Component<ModifyComponent>
	{
		Array<Modify> m_Changes;
	};
}
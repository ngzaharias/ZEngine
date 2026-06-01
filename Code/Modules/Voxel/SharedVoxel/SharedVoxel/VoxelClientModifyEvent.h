#pragma once

#include "Core/Array.h"
#include "ECS/Event.h"
#include "ECS/IsReplicated.h"
#include "Math/Vector.h"
#include "SharedVoxel/VoxelTypes.h"

namespace shared::voxel
{
	struct Modify 
	{ 
		Vector3f m_WorldPos = {};
		shared::voxel::Block m_Data = {};
	};

	struct ClientModifyEvent final : public ecs::Event
		, ecs::IsReplicated
	{
		Array<Modify> m_Changes = {};
	};
}
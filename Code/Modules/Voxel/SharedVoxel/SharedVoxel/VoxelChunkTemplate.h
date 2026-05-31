#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "SharedVoxel/VoxelTypes.h"

namespace voxel
{
	struct ChunkTemplate final : public ecs::TemplateComponent
	{
		Array<voxel::Block> m_Data;
	};
}
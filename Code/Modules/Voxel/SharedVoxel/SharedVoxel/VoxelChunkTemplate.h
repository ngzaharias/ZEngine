#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "SharedVoxel/VoxelTypes.h"

namespace shared::voxel
{
	struct ChunkTemplate final : public ecs::TemplateComponent
	{
		Array<shared::voxel::Block> m_Data;
	};
}
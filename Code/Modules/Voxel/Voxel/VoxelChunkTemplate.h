#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Voxel/VoxelTypes.h"

namespace voxel
{
	struct ChunkTemplate final : public ecs::TemplateComponent
	{
		Array<voxel::Block> m_Data;
	};
}
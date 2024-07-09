#include "EnginePCH.h"
#include "Engine/VoxelChunkPrototype.h"

#include "ECS/EntityWorld.h"
#include "Engine/Visitor.h"
#include "Engine/VoxelComponents.h"

namespace
{
	const str::StringView strData = "m_Data";
	const str::StringView strFlags = "m_Flags";
	const str::StringView strType = "m_Type";
}

void eng::VoxelChunkLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const VoxelChunkPrototype& prototype) const
{
	PROFILE_FUNCTION();

	auto& component = world.AddComponent<voxel::ChunkComponent>(entity);
	component.m_Data = prototype.m_Data;
}

void eng::VoxelChunkLoader::Load(VoxelChunkPrototype& prototype, eng::Visitor& visitor) const
{
	PROFILE_FUNCTION();

	Array<voxel::EType> values;
	visitor.Visit(strData, values, {});
	for (const voxel::EType value : values)
	{
		voxel::Block& block = prototype.m_Data.Emplace();
		block.m_Type = value;
	}
}
#include "EnginePCH.h"
#include "Engine/VoxelChunkPrototype.h"

#include <ECS/EntityWorld.h>

#include "Engine/Visitor.h"
#include "Engine/VoxelComponents.h"

namespace
{
	const str::Name strData = NAME("m_Data");
	const str::Name strFlags = NAME("m_Flags");
	const str::Name strType = NAME("m_Type");
}

void eng::VoxelChunkPrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const VoxelChunkPrototype& prototype) const
{
	PROFILE_FUNCTION();

	auto& component = world.AddComponent<voxel::ChunkComponent>(entity);
	component.m_Data = prototype.m_Data;
}

void eng::VoxelChunkPrototypeLoader::Load(VoxelChunkPrototype& prototype, eng::Visitor& visitor) const
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
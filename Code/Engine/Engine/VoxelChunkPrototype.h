#pragma once

#include <Core/Array.h>

#include <Engine/PrototypeLoader.h>
#include <Engine/VoxelTypes.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;

	struct VoxelChunkPrototype final
	{
		Array<voxel::Block> m_Data;
	};

	class VoxelChunkLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::VoxelChunkPrototype& prototype) const;
		void Load(eng::VoxelChunkPrototype& prototype, eng::Visitor& visitor) const;
	};
}
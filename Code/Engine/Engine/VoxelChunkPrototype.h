#pragma once

#include <Core/Array.h>

#include <Engine/Prototype.h>
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

	struct VoxelChunkPrototype final : public eng::Prototype
	{
		Array<voxel::Block> m_Data;
	};

	class VoxelChunkPrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const VoxelChunkPrototype& prototype) const;
		void Load(VoxelChunkPrototype& prototype, eng::Visitor& visitor) const;
	};
}
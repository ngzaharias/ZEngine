#pragma once

#include <Core/Guid.h>

#include <Engine/Prototype.h>
#include <Engine/PrototypeLoader.h>
#include <Engine/TomlTypes.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;

	struct StaticMeshPrototype final : public eng::Prototype
	{
		str::Guid m_StaticMesh = { };
	};

	class StaticMeshPrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const StaticMeshPrototype& prototype) const;
		void Load(StaticMeshPrototype& prototype, eng::Visitor& visitor) const;
	};
}
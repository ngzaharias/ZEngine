#pragma once

#include <Core/Guid.h>

#include <Engine/PrototypeLoader.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;

	struct StaticMeshPrototype final
	{
		str::Guid m_StaticMesh = { };
	};

	class StaticMeshLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const StaticMeshPrototype& prototype) const;
		void Load(StaticMeshPrototype& prototype, eng::Visitor& visitor) const;
	};
}
#pragma once

#include <Core/String.h>

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

	struct NamePrototype final : public eng::Prototype
	{
	};

	class NamePrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const NamePrototype& prototype) const;
		void Load(NamePrototype& prototype, eng::Visitor& visitor) const;
	};
}
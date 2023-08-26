#pragma once

#include <Core/Guid.h>
#include <Core/Vector.h>

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

	struct FlipbookPrototype final : public eng::Prototype
	{
		Vector2u m_Size = Vector2u::Zero;
		str::Guid m_Flipbook = { };
	};

	class FlipbookPrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const FlipbookPrototype& prototype) const;
		void Load(FlipbookPrototype& prototype, eng::Visitor& visitor) const;
	};
}
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

	struct SpritePrototype final : public eng::Prototype
	{
		Vector2u m_Size = Vector2u::Zero;
		str::Guid m_Sprite = { };
	};

	class SpritePrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const SpritePrototype& prototype) const;
		void Load(SpritePrototype& prototype, eng::Visitor& visitor) const;
	};
}
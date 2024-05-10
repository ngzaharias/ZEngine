#pragma once

#include <Core/Guid.h>
#include <Core/Vector.h>

#include <Engine/Prototype.h>
#include <Engine/PrototypeLoader.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;
}

namespace hidden
{
	struct ObjectPrototype final : public eng::Prototype
	{
		Vector2u m_Size = Vector2u::Zero;
		str::Guid m_Sprite = {};
	};

	class ObjectPrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const hidden::ObjectPrototype& prototype) const;

		void Load(hidden::ObjectPrototype& prototype, eng::Visitor& visitor) const;
	};
}
#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Engine/PrototypeLoader.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

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
	struct GroupPrototype final
	{
		Array<str::Guid> m_Objects = {};
		Vector2u m_Size = Vector2u::Zero;
		str::Guid m_Sprite = {};
	};

	class GroupLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const hidden::GroupPrototype& prototype) const;

		void Load(hidden::GroupPrototype& prototype, eng::Visitor& visitor) const;
	};

	struct ObjectPrototype final
	{
		str::Guid m_Group = {};
		str::Guid m_Sprite = {};
		Vector2u m_Size = Vector2u::Zero;
	};

	class ObjectLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const hidden::ObjectPrototype& prototype) const;

		void Load(hidden::ObjectPrototype& prototype, eng::Visitor& visitor) const;
	};
}
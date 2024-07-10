#pragma once

#include "Core/Guid.h"
#include "Engine/PrototypeLoader.h"
#include "Math/Vector.h"

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;

	struct FlipbookPrototype final
	{
		Vector2u m_Size = Vector2u::Zero;
		str::Guid m_Flipbook = { };
	};

	class FlipbookLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::FlipbookPrototype& prototype) const;
		void Load(eng::FlipbookPrototype& prototype, eng::Visitor& visitor) const;
	};
}
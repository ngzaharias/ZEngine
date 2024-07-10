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

	struct SpritePrototype final
	{
		Vector2u m_Size = Vector2u::Zero;
		str::Guid m_Sprite = { };
	};

	class SpriteLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::SpritePrototype& prototype) const;
		void Load(eng::SpritePrototype& prototype, eng::Visitor& visitor) const;
	};
}
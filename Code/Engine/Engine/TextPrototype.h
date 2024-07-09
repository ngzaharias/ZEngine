#pragma once

#include "Core/String.h"
#include "Core/Vector.h"
#include "Engine/PrototypeLoader.h"

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;

	struct TextPrototype final
	{
		str::String m_Text = { };
		str::Guid m_Font = { };
	};

	class TextLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::TextPrototype& prototype) const;
		void Load(eng::TextPrototype& prototype, eng::Visitor& visitor) const;
	};
}
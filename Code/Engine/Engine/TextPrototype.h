#pragma once

#include <Core/String.h>
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

	struct TextPrototype final : public eng::Prototype
	{
		str::String m_Text = { };
		str::Guid m_Font = { };
	};

	class TextPrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const TextPrototype& prototype) const;
		void Load(TextPrototype& prototype, eng::Visitor& visitor) const;
	};
}
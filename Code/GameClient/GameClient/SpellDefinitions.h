#pragma once

#include <Core/Array.h>
#include <Core/Guid.h>
#include <Core/Map.h>
#include <Core/Set.h>
#include <Core/Variant.h>

#include <ECS/Entity.h>

namespace spell
{
	struct SubDefinition;

	struct Damage {};
	struct Explosion {};
	struct Projectile {};

	using Evocation = Variant<
		Damage,
		Explosion,
		Projectile>;

	using SpellId = str::Guid;
	using SubIndex = int32;

	struct RootDefinition
	{
		/// \brief All sub-spells that are invoked first.
		Array<SubIndex> m_Children = {};

		/// \brief Container for every sub-spell of the spell.
		Array<SubDefinition> m_SubDefinitions = {};
	};

	struct SubDefinition
	{
		/// \brief All sub-spells that are invoked next.
		Array<SubIndex> m_Children = {};

		Evocation m_Evocation = {};
	};
}
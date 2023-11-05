#pragma once

#include <Core/Guid.h>
#include <Core/Map.h>

#include <ECS/Component.h>
#include <ECS/Entity.h>

#include <GameClient/SpellDefinitions.h>

namespace spell
{
	struct DefinitionMapComponent : public ecs::SingletonComponent<DefinitionMapComponent>
	{
		Map<str::Guid, spell::RootDefinition> m_GuidToSpell = {};
	};

	struct RequestComponent : public ecs::Component<RequestComponent>
	{
		str::Guid TransactionId = {};
		spell::SpellId m_SpellId = {};
	};

	struct RootComponent : public ecs::Component<RootComponent>
	{
		spell::SpellId m_SpellId = {};
		Array<ecs::Entity> m_SubSpells = {};
	};

	struct SubComponent : public ecs::Component<SubComponent>
	{
		ecs::Entity m_Root = {};
		ecs::Entity m_Parent = {};

		spell::SpellId m_SpellId = {};
		spell::SubIndex m_SubIndex = -1;
		float m_Lifetime = 0.f;
		float m_Timeout = FLT_MAX;
	};
}
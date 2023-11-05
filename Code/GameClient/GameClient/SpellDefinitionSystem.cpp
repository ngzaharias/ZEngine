#include "GameClientPCH.h"
#include "GameClient/SpellDefinitionSystem.h"

#include <Core/VariantHelpers.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "GameClient/SpellComponents.h"
#include "GameClient/SpellDefinitions.h"

namespace
{
	const str::Guid strGuidA = str::Guid::Generate();
	const str::Guid strGuidB = str::Guid::Generate();
}

void spell::DefinitionSystem::Initialise(World& world)
{
	auto& component = world.AddSingleton<spell::DefinitionMapComponent>();

	if (true)
	{
		auto& definition = component.m_GuidToSpell[strGuidA];
		definition.m_Children.Append(0);
		auto& subspellA = definition.m_SubDefinitions.Emplace();
		subspellA.m_Evocation = spell::Projectile();
		subspellA.m_Children.Append(1);
		auto& subspellB = definition.m_SubDefinitions.Emplace();
		subspellB.m_Evocation = spell::Explosion();
		subspellB.m_Children.Append(2);
		auto& subspellC = definition.m_SubDefinitions.Emplace();
		subspellC.m_Evocation = spell::Damage();
	}

	if (true)
	{
		auto& definition = component.m_GuidToSpell[strGuidB];
		definition.m_Children.Append(0);
		definition.m_Children.Append(1);
		auto& subspellA = definition.m_SubDefinitions.Emplace();
		subspellA.m_Evocation = spell::Projectile();
		subspellA.m_Children.Append(2);
		subspellA.m_Children.Append(3);
		auto& subspellB = definition.m_SubDefinitions.Emplace();
		subspellB.m_Evocation = spell::Projectile();
		subspellB.m_Children.Append(4);
		subspellB.m_Children.Append(5);

		auto& subspellAA = definition.m_SubDefinitions.Emplace();
		subspellAA.m_Evocation = spell::Damage();
		auto& subspellAB = definition.m_SubDefinitions.Emplace();
		subspellAB.m_Evocation = spell::Damage();

		auto& subspellBA = definition.m_SubDefinitions.Emplace();
		subspellBA.m_Evocation = spell::Damage();
		auto& subspellBB = definition.m_SubDefinitions.Emplace();
		subspellBB.m_Evocation = spell::Damage();
	}
}

void spell::DefinitionSystem::Shutdown(World& world)
{
	world.RemoveSingleton<spell::DefinitionMapComponent>();
}

void spell::DefinitionSystem::Update(World& world, const GameTime& gameTime)
{
}
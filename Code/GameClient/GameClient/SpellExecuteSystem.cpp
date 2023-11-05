#include "GameClientPCH.h"
#include "GameClient/SpellExecuteSystem.h"

#include <Core/VariantHelpers.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "GameClient/SpellComponents.h"
#include "GameClient/SpellDefinitions.h"

namespace
{
	const spell::SubDefinition& GetSubSpell(const spell::RootDefinition& definition, const spell::SubIndex& index)
	{
		return definition.m_SubDefinitions[index];
	}
}

void spell::ExecuteSystem::Update(World& world, const GameTime& gameTime)
{
	static bool execute = true;
	if (execute)
	{
		execute = false;

		const auto& definitionComponent = world.GetSingleton<const spell::DefinitionMapComponent>();

		struct Command
		{
			Command(const RootDefinition& spell, const SubDefinition& subSpell)
				: m_Spell(spell), m_SubSpell(subSpell)
			{}

			const RootDefinition& m_Spell;
			const SubDefinition& m_SubSpell;
		};

		Array<Command> toExecute;
		for (auto&& [guid, spell] : definitionComponent.m_GuidToSpell)
		{
			for (const spell::SubIndex& index : spell.m_Children)
			{
				const spell::SubDefinition& subSpell = GetSubSpell(spell, index);
				toExecute.Emplace(spell, subSpell);
			}
		}

		// commands are executed LIFO, which means a sub-spell is 
		// expanded fully before it moves onto the next sub-spell
		while(!toExecute.IsEmpty())
		{
			// copy since we modify the array
			const Command command = toExecute.Pop();

			for (const spell::SubIndex& index : command.m_SubSpell.m_Children)
			{
				const spell::SubDefinition& subSpell = GetSubSpell(command.m_Spell, index);
				toExecute.Emplace(command.m_Spell, subSpell);
			}
		}
	}
}
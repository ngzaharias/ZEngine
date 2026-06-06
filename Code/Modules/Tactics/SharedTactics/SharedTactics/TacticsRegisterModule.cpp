#include "TacticsPCH.h"
#include "SharedTactics/TacticsRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/TemplateManager.h"
#include "SharedTactics/TacticsPawnAbilitiesTemplate.h"
#include "SharedTactics/TacticsAbilityTable.h"

void shared::tactics::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::tactics::PawnAbilitiesTemplate>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<shared::tactics::PawnAbilitiesTemplate>();
	}

	// tables
	{
		auto& headmaster = world.WriteResource<eng::TableHeadmaster>();
		headmaster.Register<shared::tactics::AbilityTable>("Abilities");

		world.RegisterResource(headmaster.GetManager<shared::tactics::AbilityTable>());
	}
}
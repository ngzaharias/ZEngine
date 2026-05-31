#include "TacticsPCH.h"
#include "SharedTactics/TacticsRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/TemplateManager.h"
#include "SharedTactics/TacticsPawnAbilitiesComponent.h"
#include "SharedTactics/TacticsPawnAbilitiesTemplate.h"
#include "SharedTactics/TacticsAbilityPreviewComponent.h"
#include "SharedTactics/TacticsAbilityPreviewEvent.h"
#include "SharedTactics/TacticsAbilityPreviewSystem.h"
#include "SharedTactics/TacticsAbilityTable.h"
#include "SharedTactics/TacticsPawnSelectedComponent.h"
#include "SharedTactics/TacticsPawnSelectionSystem.h"

void tactics::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<tactics::AbilityPreviewComponent>();
	world.RegisterComponent<tactics::PawnAbilitiesComponent>();
	world.RegisterComponent<tactics::PawnAbilitiesTemplate>();
	world.RegisterComponent<tactics::PawnSelectedComponent>();
	world.RegisterEvent<tactics::AbilityPreviewEvent>();
	world.RegisterSystem<tactics::AbilityPreviewSystem>();
	world.RegisterSystem<tactics::PawnSelectionSystem>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<tactics::PawnAbilitiesTemplate>();
	}

	// tables
	{
		auto& headmaster = world.WriteResource<eng::TableHeadmaster>();
		headmaster.Register<tactics::AbilityTable>("Abilities");

		world.RegisterResource(headmaster.GetManager<tactics::AbilityTable>());
	}
}
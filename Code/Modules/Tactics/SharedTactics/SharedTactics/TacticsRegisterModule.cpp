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

void shared::tactics::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::tactics::AbilityPreviewComponent>();
	world.RegisterComponent<shared::tactics::PawnAbilitiesComponent>();
	world.RegisterComponent<shared::tactics::PawnAbilitiesTemplate>();
	world.RegisterComponent<shared::tactics::PawnSelectedComponent>();
	world.RegisterEvent<shared::tactics::AbilityPreviewEvent>();
	world.RegisterSystem<shared::tactics::AbilityPreviewSystem>();
	world.RegisterSystem<shared::tactics::PawnSelectionSystem>();

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
#include "TacticsPCH.h"
#include "Tactics/TacticsRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/TemplateManager.h"
#include "Tactics/TacticsPawnAbilitiesComponent.h"
#include "Tactics/TacticsPawnAbilitiesTemplate.h"
#include "Tactics/TacticsAbilityPreviewComponent.h"
#include "Tactics/TacticsAbilityPreviewEvent.h"
#include "Tactics/TacticsAbilityPreviewSystem.h"
#include "Tactics/TacticsAbilityTable.h"
#include "Tactics/TacticsPawnSelectedComponent.h"
#include "Tactics/TacticsPawnSelectionSystem.h"

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
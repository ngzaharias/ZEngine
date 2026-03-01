#include "TacticsPCH.h"
#include "Tactics/TacticsRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Engine/TableHeadmaster.h"
#include "Tactics/TacticsAbilityComponent.h"
#include "Tactics/TacticsAbilityTable.h"
#include "Tactics/TacticsExecuteComponent.h"
#include "Tactics/TacticsExecuteEvent.h"
#include "Tactics/TacticsInputSystem.h"
#include "Tactics/TacticsPreviewComponent.h"
#include "Tactics/TacticsPreviewEvent.h"
#include "Tactics/TacticsPreviewSystem.h"
#include "Tactics/TacticsSelectedComponent.h"
#include "Tactics/TacticsSelectSystem.h"

void tactics::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<tactics::AbilityComponent>();
	world.RegisterComponent<tactics::ExecuteComponent>();
	world.RegisterComponent<tactics::PreviewComponent>();
	world.RegisterComponent<tactics::SelectedComponent>();
	world.RegisterEvent<tactics::ExecuteEvent>();
	world.RegisterEvent<tactics::PreviewEvent>();
	world.RegisterSystem<tactics::InputSystem>();
	world.RegisterSystem<tactics::PreviewSystem>();
	world.RegisterSystem<tactics::SelectSystem>();

	// prototypes
	{
		auto& manager = world.WriteResource<eng::PrototypeManager>();
		manager.RegisterComponent<tactics::AbilityComponent>();
	}

	// tables
	{
		auto& headmaster = world.WriteResource<eng::TableHeadmaster>();
		headmaster.Register<tactics::AbilityTable>("Abilities");

		world.RegisterResource(headmaster.GetManager<tactics::AbilityTable>());
	}
}
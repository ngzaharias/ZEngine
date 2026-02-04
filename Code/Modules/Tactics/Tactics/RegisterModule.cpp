#include "TacticsPCH.h"
#include "Tactics/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Engine/TableHeadmaster.h"
#include "Tactics/TacticsAbilityComponent.h"
#include "Tactics/TacticsAbilityTable.h"
#include "Tactics/TacticsInputSystem.h"
#include "Tactics/TacticsPreviewSystem.h"
#include "Tactics/TacticsSelectedComponent.h"
#include "Tactics/TacticsSelectSystem.h"

void tactics::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<tactics::AbilityComponent>();
		world.RegisterComponent<tactics::SelectedComponent>();
	}

	// systems
	{
		world.RegisterSystem<tactics::InputSystem>();
		world.RegisterSystem<tactics::PreviewSystem>();
		world.RegisterSystem<tactics::SelectSystem>();
	}

	// prototypes
	{
		auto& manager = entityWorld.WriteResource<eng::PrototypeManager>();
		manager.Register<tactics::AbilityComponent>();
	}

	// tables
	{
		auto& headmaster = entityWorld.WriteResource<eng::TableHeadmaster>();
		headmaster.Register<tactics::AbilityTable>("Abilities");

		world.RegisterResource(headmaster.GetManager<tactics::AbilityTable>());
	}
}
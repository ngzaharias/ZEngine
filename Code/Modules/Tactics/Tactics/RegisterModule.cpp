#include "TacticsPCH.h"
#include "Tactics/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Engine/TableHeadmaster.h"
#include "Tactics/TacticsAbilityComponent.h"
#include "Tactics/TacticsAbilityTable.h"
#include "Tactics/TacticsInputSystem.h"
#include "Tactics/TacticsSelectSystem.h"
#include "Tactics/TacticsPreviewSystem.h"
#include "Tactics/TacticsSelectedComponent.h"

void tactics::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<tactics::AbilityComponent>();
		entityWorld.RegisterComponent<tactics::SelectedComponent>();
	}

	// systems
	{
		entityWorld.RegisterSystem<tactics::InputSystem>();
		entityWorld.RegisterSystem<tactics::PreviewSystem>();
		entityWorld.RegisterSystem<tactics::SelectSystem>();
	}

	// prototypes
	{
		auto& prototypeManager = entityWorld.WriteResource<eng::PrototypeManager>();
		prototypeManager.Register<tactics::AbilityComponent>();
	}

	// tables
	{
		auto& tableHeadmaster = entityWorld.WriteResource<eng::TableHeadmaster>();
		tableHeadmaster.Register<tactics::SkillTable>("Skills");
		auto& headmaster = entityWorld.WriteResource<eng::TableHeadmaster>();
		headmaster.Register<tactics::AbilityTable>("Abilities");

		entityWorld.RegisterResource(headmaster.GetManager<tactics::AbilityTable>());

	}
}
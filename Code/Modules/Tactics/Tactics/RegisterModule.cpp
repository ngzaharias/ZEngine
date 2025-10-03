#include "TacticsPCH.h"
#include "Tactics/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TableHeadmaster.h"
#include "Tactics/TacticsInputSystem.h"
#include "Tactics/TacticsSelectSystem.h"
#include "Tactics/TacticsSelectedComponent.h"
#include "Tactics/TacticsSkillTable.h"

void tactics::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<tactics::SelectedComponent>();
	}

	// systems
	{
		entityWorld.RegisterSystem<tactics::InputSystem>();
		entityWorld.RegisterSystem<tactics::SelectSystem>();
	}

	// tables
	{
		auto& tableHeadmaster = entityWorld.WriteResource<eng::TableHeadmaster>();
		tableHeadmaster.Register<tactics::SkillTable>("Skills");
	}
}
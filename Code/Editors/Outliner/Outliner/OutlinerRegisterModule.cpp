#include "OutlinerPCH.h"
#include "Outliner/OutlinerRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Outliner/OutlinerOpenWindowEvent.h"
#include "Outliner/OutlinerSelectSingleton.h"
#include "Outliner/OutlinerSelectSystem.h"
#include "Outliner/OutlinerWindowComponent.h"
#include "Outliner/OutlinerWindowSystem.h"

void editor::outliner::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::outliner::WindowComponent>();
	world.RegisterEvent<editor::outliner::OpenWindowEvent>();
	world.RegisterSingleton<editor::outliner::SelectSingleton>();
	world.RegisterSystem<editor::outliner::SelectSystem>();
	world.RegisterSystem<editor::outliner::WindowSystem>();
}
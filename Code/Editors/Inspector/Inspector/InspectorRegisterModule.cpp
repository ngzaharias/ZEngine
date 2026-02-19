#include "InspectorPCH.h"
#include "Inspector/InspectorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Inspector/InspectorOpenWindowEvent.h"
#include "Inspector/InspectorSaveComponent.h"
#include "Inspector/InspectorSettingsSingleton.h"
#include "Inspector/InspectorSettingsSystem.h"
#include "Inspector/InspectorWindowComponent.h"
#include "Inspector/InspectorWindowSystem.h"

void editor::inspector::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::inspector::SaveComponent>();
	world.RegisterComponent<editor::inspector::WindowComponent>();
	world.RegisterEvent<editor::inspector::OpenWindowEvent>();
	world.RegisterSingleton<editor::inspector::SettingsSingleton>();
	world.RegisterSystem<editor::inspector::SettingsSystem>();
	world.RegisterSystem<editor::inspector::WindowSystem>(world);
}
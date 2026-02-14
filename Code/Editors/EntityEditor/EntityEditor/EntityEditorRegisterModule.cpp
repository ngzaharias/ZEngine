#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "EntityEditor/EntityEditorOpenWindowEvent.h"
#include "EntityEditor/EntityEditorSaveComponent.h"
#include "EntityEditor/EntityEditorSelectSingleton.h"
#include "EntityEditor/EntityEditorSelectSystem.h"
#include "EntityEditor/EntityEditorSettingsSingleton.h"
#include "EntityEditor/EntityEditorSettingsSystem.h"
#include "EntityEditor/EntityEditorWindowComponent.h"
#include "EntityEditor/EntityEditorWindowSystem.h"

void editor::entity::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::entity::SaveComponent>();
	world.RegisterComponent<editor::entity::WindowComponent>();
	world.RegisterEvent<editor::entity::OpenWindowEvent>();
	world.RegisterSingleton<editor::entity::SelectSingleton>();
	world.RegisterSingleton<editor::entity::SettingsSingleton>();
	world.RegisterSystem<editor::entity::SelectSystem>();
	world.RegisterSystem<editor::entity::SettingsSystem>();
	world.RegisterSystem<editor::entity::WindowSystem>(world);
}
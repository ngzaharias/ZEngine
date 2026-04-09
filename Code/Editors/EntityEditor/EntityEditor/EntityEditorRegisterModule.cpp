#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "EntityEditor/EntityEditorCommandManager.h"
#include "EntityEditor/EntityEditorInspectorComponent.h"
#include "EntityEditor/EntityEditorInspectorSystem.h"
#include "EntityEditor/EntityEditorOpenInspectorEvent.h"
#include "EntityEditor/EntityEditorOpenOutlinerEvent.h"
#include "EntityEditor/EntityEditorOutlinerComponent.h"
#include "EntityEditor/EntityEditorOutlinerSystem.h"
#include "EntityEditor/EntityEditorSelectComponent.h"
#include "EntityEditor/EntityEditorSelectSystem.h"
#include "EntityEditor/EntityEditorSettingsComponent.h"
#include "EntityEditor/EntityEditorSettingsSystem.h"

void editor::entity::RegisterModule(ecs::EntityWorld& world)
{
	static editor::entity::CommandManager manager(world);

	world.RegisterComponent<editor::entity::InspectorComponent>();
	world.RegisterComponent<editor::entity::OutlinerComponent>();
	world.RegisterComponent<editor::entity::SelectComponent>();
	world.RegisterComponent<editor::entity::SettingsComponent>();
	world.RegisterEvent<editor::entity::OpenInspectorEvent>();
	world.RegisterEvent<editor::entity::OpenOutlinerEvent>();
	world.RegisterResource<editor::entity::CommandManager>(manager);
	world.RegisterSystem<editor::entity::InspectorSystem>(world);
	world.RegisterSystem<editor::entity::OutlinerSystem>(world);
	world.RegisterSystem<editor::entity::SelectSystem>();
	world.RegisterSystem<editor::entity::SettingsSystem>();
}
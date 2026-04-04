#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "EntityEditor/EntityEditorEntityCreatedEvent.h"
#include "EntityEditor/EntityEditorEntityDestroyedEvent.h"
#include "EntityEditor/EntityEditorEntitySelectedEvent.h"
#include "EntityEditor/EntityEditorEntityUpdatedEvent.h"
#include "EntityEditor/EntityEditorHistoryComponent.h"
#include "EntityEditor/EntityEditorHistoryRedoEvent.h"
#include "EntityEditor/EntityEditorHistorySystem.h"
#include "EntityEditor/EntityEditorHistoryUndoEvent.h"
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
	world.RegisterComponent<editor::entity::HistoryComponent>();
	world.RegisterComponent<editor::entity::InspectorComponent>();
	world.RegisterComponent<editor::entity::OutlinerComponent>();
	world.RegisterComponent<editor::entity::SelectComponent>();
	world.RegisterComponent<editor::entity::SettingsComponent>();
	world.RegisterEvent<editor::entity::EntityCreatedEvent>();
	world.RegisterEvent<editor::entity::EntityDestroyedEvent>();
	world.RegisterEvent<editor::entity::EntitySelectedEvent>();
	world.RegisterEvent<editor::entity::EntityUpdatedEvent>();
	world.RegisterEvent<editor::entity::HistoryRedoEvent>();
	world.RegisterEvent<editor::entity::HistoryUndoEvent>();
	world.RegisterEvent<editor::entity::OpenInspectorEvent>();
	world.RegisterEvent<editor::entity::OpenOutlinerEvent>();
	world.RegisterSystem<editor::entity::HistorySystem>(world);
	world.RegisterSystem<editor::entity::InspectorSystem>(world);
	world.RegisterSystem<editor::entity::OutlinerSystem>();
	world.RegisterSystem<editor::entity::SelectSystem>();
	world.RegisterSystem<editor::entity::SettingsSystem>();
}
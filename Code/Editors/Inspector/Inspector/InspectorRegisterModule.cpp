#include "InspectorPCH.h"
#include "Inspector/InspectorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Inspector/InspectorHistoryComponent.h"
#include "Inspector/InspectorHistoryCreateEvent.h"
#include "Inspector/InspectorHistoryDestroyEvent.h"
#include "Inspector/InspectorHistoryRedoEvent.h"
#include "Inspector/InspectorHistorySystem.h"
#include "Inspector/InspectorHistoryUndoEvent.h"
#include "Inspector/InspectorHistoryUpdateEvent.h"
#include "Inspector/InspectorOpenWindowEvent.h"
#include "Inspector/InspectorSaveComponent.h"
#include "Inspector/InspectorSettingsComponent.h"
#include "Inspector/InspectorSettingsSystem.h"
#include "Inspector/InspectorWindowComponent.h"
#include "Inspector/InspectorWindowSystem.h"

void editor::inspector::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::inspector::HistoryComponent>();
	world.RegisterComponent<editor::inspector::SaveComponent>();
	world.RegisterComponent<editor::inspector::SettingsComponent>();
	world.RegisterComponent<editor::inspector::WindowComponent>();
	world.RegisterEvent<editor::inspector::HistoryCreateEvent>();
	world.RegisterEvent<editor::inspector::HistoryDestroyEvent>();
	world.RegisterEvent<editor::inspector::HistoryRedoEvent>();
	world.RegisterEvent<editor::inspector::HistoryUndoEvent>();
	world.RegisterEvent<editor::inspector::HistoryUpdateEvent>();
	world.RegisterEvent<editor::inspector::OpenWindowEvent>();
	world.RegisterSystem<editor::inspector::HistorySystem>(world);
	world.RegisterSystem<editor::inspector::SettingsSystem>();
	world.RegisterSystem<editor::inspector::WindowSystem>(world);
}
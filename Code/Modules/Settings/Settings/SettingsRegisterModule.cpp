#include "Settings/SettingsRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "Settings/SettingsEditorComponent.h"
#include "Settings/SettingsEditorToggleEvent.h"
#include "Settings/SettingsEditorSystem.h"

void settings::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<settings::EditorComponent>();
	world.RegisterEvent<settings::EditorToggleEvent>();
	world.RegisterSystem<settings::EditorSystem>();
}
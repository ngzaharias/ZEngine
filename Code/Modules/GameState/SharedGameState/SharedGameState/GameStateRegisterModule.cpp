#include "SharedGameState/GameStateRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "SharedGameState/GameStateDebugComponent.h"
#include "SharedGameState/GameStateDebugSystem.h"
#include "SharedGameState/GameStateEditModeComponent.h"
#include "SharedGameState/GameStateEditModeSystem.h"
#include "SharedGameState/GameStateEditModeToggleEvent.h"
#include "SharedGameState/GameStateEditorComponent.h"
#include "SharedGameState/GameStateEditorSystem.h"

void gamestate::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<gamestate::DebugComponent>();
	world.RegisterComponent<gamestate::EditModeComponent>();
	world.RegisterComponent<gamestate::EditorComponent>();
	world.RegisterEvent<gamestate::EditModeToggleEvent>();
	world.RegisterSystem<gamestate::DebugSystem>();
	world.RegisterSystem<gamestate::EditModeSystem>();
	world.RegisterSystem<gamestate::EditorSystem>();
}
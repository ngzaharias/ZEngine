#include "GameState/GameStateRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "GameState/GameStateDebugComponent.h"
#include "GameState/GameStateDebugSystem.h"
#include "GameState/GameStateEditModeComponent.h"
#include "GameState/GameStateEditModeSystem.h"
#include "GameState/GameStateEditModeToggleEvent.h"
#include "GameState/GameStateEditorComponent.h"
#include "GameState/GameStateEditorSystem.h"

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
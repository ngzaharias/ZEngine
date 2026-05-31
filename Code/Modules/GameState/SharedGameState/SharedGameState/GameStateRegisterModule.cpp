#include "SharedGameState/GameStateRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "SharedGameState/GameStateDebugComponent.h"
#include "SharedGameState/GameStateDebugSystem.h"
#include "SharedGameState/GameStateEditModeComponent.h"
#include "SharedGameState/GameStateEditModeSystem.h"
#include "SharedGameState/GameStateEditModeToggleEvent.h"
#include "SharedGameState/GameStateEditorComponent.h"
#include "SharedGameState/GameStateEditorSystem.h"

void shared::gamestate::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::gamestate::DebugComponent>();
	world.RegisterComponent<shared::gamestate::EditModeComponent>();
	world.RegisterComponent<shared::gamestate::EditorComponent>();
	world.RegisterEvent<shared::gamestate::EditModeToggleEvent>();
	world.RegisterSystem<shared::gamestate::DebugSystem>();
	world.RegisterSystem<shared::gamestate::EditModeSystem>();
	world.RegisterSystem<shared::gamestate::EditorSystem>();
}
#include "GameState/GameStateRegisterModule.h"

#include "GameState/GameStateEditModeComponent.h"
#include "GameState/GameStateEditModeToggleEvent.h"
#include "GameState/GameStateEditModeSystem.h"
#include "ECS/EntityWorld.h"

void gamestate::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<gamestate::EditModeComponent>();
	world.RegisterEvent<gamestate::EditModeToggleEvent>();
	world.RegisterSystem<gamestate::EditModeSystem>();
}
#include "GameState/GameStateEditModeSystem.h"

#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameState/GameStateEditModeComponent.h"
#include "GameState/GameStateEditModeToggleEvent.h"

void gamestate::EditModeSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

#ifdef Z_EDITOR
	world.AddComponent<gamestate::EditModeComponent>();
#endif
}

void gamestate::EditModeSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<gamestate::EditModeToggleEvent>())
	{
		if (!world.HasComponent<gamestate::EditModeComponent>())
		{
			world.AddComponent<gamestate::EditModeComponent>();
		}
		else
		{
			world.RemoveComponent<gamestate::EditModeComponent>();
		}
	}
}
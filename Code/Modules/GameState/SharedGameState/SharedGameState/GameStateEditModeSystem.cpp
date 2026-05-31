#include "SharedGameState/GameStateEditModeSystem.h"

#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "SharedGameState/GameStateEditModeComponent.h"
#include "SharedGameState/GameStateEditModeToggleEvent.h"

void shared::gamestate::EditModeSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

#ifdef Z_EDITOR
	world.AddComponent<shared::gamestate::EditModeComponent>();
#endif
}

void shared::gamestate::EditModeSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<shared::gamestate::EditModeToggleEvent>())
	{
		if (!world.HasComponent<shared::gamestate::EditModeComponent>())
		{
			world.AddComponent<shared::gamestate::EditModeComponent>();
		}
		else
		{
			world.RemoveComponent<shared::gamestate::EditModeComponent>();
		}
	}
}
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
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<gamestate::EditModeComponent>(entity);
#endif
}

void gamestate::EditModeSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<gamestate::EditModeToggleEvent>())
	{
		if (world.HasAny<ecs::query::Include<gamestate::EditModeComponent>>())
		{
			for (auto&& view : world.Query<ecs::query::Include<gamestate::EditModeComponent>>())
				world.DestroyEntity(view);
		}
		else
		{
			const ecs::Entity entity = world.CreateEntity();
			world.AddComponent<gamestate::EditModeComponent>(entity);
		}
	}
}
#include "SharedGameState/GameStateDebugSystem.h"

#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "SharedGameState/GameStateDebugComponent.h"

void shared::gamestate::DebugSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

#ifdef Z_DEBUG
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<shared::gamestate::DebugComponent>(entity);
#endif
}
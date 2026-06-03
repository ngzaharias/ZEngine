#include "EnginePCH.h"
#include "Engine/ServerSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ServerComponent.h"

void eng::ServerSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	world.AddComponent<eng::ServerComponent>();
}

void eng::ServerSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	world.RemoveComponent<eng::ServerComponent>();
}
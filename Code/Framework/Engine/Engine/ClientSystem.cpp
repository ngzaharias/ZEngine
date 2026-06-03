#include "EnginePCH.h"
#include "Engine/ClientSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ClientComponent.h"

void eng::ClientSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	world.AddComponent<eng::ClientComponent>();
}

void eng::ClientSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	world.RemoveComponent<eng::ClientComponent>();
}
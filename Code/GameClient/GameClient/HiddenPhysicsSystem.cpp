#include "GameClientPCH.h"
#include "GameClient/HiddenPhysicsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/PhysicsComponent.h"
#include "GameClient/HiddenObjectComponent.h"
#include "GameClient/HiddenRevealComponent.h"

void hidden::PhysicsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using Query = ecs::query
		::Added<const hidden::RevealComponent>
		::Include<const eng::PhysicsComponent>;

	for (const ecs::Entity& entity : world.Query<Query>())
	{
		world.RemoveComponent<eng::PhysicsComponent>(entity);
	}
}
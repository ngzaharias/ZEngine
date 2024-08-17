#include "GameClientPCH.h"
#include "GameClient/HiddenPhysicsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/RigidStaticComponent.h"
#include "GameClient/HiddenObjectComponents.h"

void hidden::PhysicsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using Query = ecs::query
		::Added<const hidden::RevealedComponent>
		::Include<const eng::RigidStaticComponent>;

	for (const ecs::Entity& entity : world.Query<Query>())
	{
		world.RemoveComponent<eng::RigidStaticComponent>(entity);
	}
}
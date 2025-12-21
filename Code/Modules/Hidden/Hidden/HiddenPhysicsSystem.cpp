#include "HiddenPCH.h"
#include "Hidden/HiddenPhysicsSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/PhysicsComponent.h"
#include "Hidden/HiddenObjectComponent.h"
#include "Hidden/HiddenRevealComponent.h"

void hidden::PhysicsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using Query = ecs::query
		::Added<const hidden::RevealComponent>
		::Include<const eng::PhysicsComponent>;

	for (auto&& view : world.Query<Query>())
	{
		world.RemoveComponent<eng::PhysicsComponent>(view);
	}
}
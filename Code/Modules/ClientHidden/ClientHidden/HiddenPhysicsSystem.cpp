#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenPhysicsSystem.h"

#include "ClientHidden/HiddenObjectComponent.h"
#include "ClientHidden/HiddenRevealComponent.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/PhysicsComponent.h"

void client::hidden::PhysicsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using Query = ecs::query
		::Added<const client::hidden::RevealComponent>
		::Include<const eng::PhysicsComponent>;

	for (auto&& view : world.Query<Query>())
	{
		world.RemoveComponent<eng::PhysicsComponent>(view);
	}
}
#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenObjectSystem.h"

#include "ClientHidden/HiddenObjectComponent.h"
#include "ClientHidden/HiddenObjectTemplate.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"

void client::hidden::ObjectSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (auto&& view : world.Query<ecs::query::Added<const client::hidden::ObjectTemplate>>())
	{
		world.AddComponent<client::hidden::ObjectComponent>(view);
	}

	for (auto&& view : world.Query<ecs::query::Removed<const client::hidden::ObjectTemplate>>())
	{
		world.RemoveComponent<client::hidden::ObjectComponent>(view);
	}
}

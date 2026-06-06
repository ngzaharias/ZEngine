#include "TilemapPCH.h"
#include "ServerTilemap/TilemapAgentSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "SharedTilemap/TilemapAgentComponent.h"
#include "SharedTilemap/TilemapAgentTemplate.h"

void server::tilemap::AgentSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using TAddedQuery = ecs::query
		::Added<const shared::tilemap::AgentTemplate>
		::Include<const shared::tilemap::AgentTemplate>;
	for (auto&& view : world.Query<TAddedQuery>())
	{
		world.AddComponent<shared::tilemap::AgentComponent>(view);
	}

	for (auto&& view : world.Query<ecs::query::Removed<const shared::tilemap::AgentTemplate>>())
	{
		world.RemoveComponent<shared::tilemap::AgentComponent>(view);
	}
}

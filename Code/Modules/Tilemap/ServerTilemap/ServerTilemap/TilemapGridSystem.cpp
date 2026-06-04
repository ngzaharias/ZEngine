#include "TilemapPCH.h"
#include "ServerTilemap/TilemapGridSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "SharedTilemap/TilemapGridComponent.h"
#include "SharedTilemap/TilemapGridTemplate.h"

void server::tilemap::GridSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using TAddedQuery = ecs::query
		::Added<const shared::tilemap::GridTemplate>
		::Include<const shared::tilemap::GridTemplate>;
	for (auto&& view : world.Query<TAddedQuery>())
	{
		const auto& gridTemplate = view.ReadRequired<shared::tilemap::GridTemplate>();
		auto& gridComponent = world.AddComponent<shared::tilemap::GridComponent>(view);
		gridComponent.m_GridSize = gridTemplate.m_GridSize;
		gridComponent.m_TileSize = gridTemplate.m_TileSize;
	}

	using TUpdatedQuery = ecs::query
		::Updated<
		const shared::tilemap::GridTemplate>
		::Include<
		const shared::tilemap::GridComponent,
		const shared::tilemap::GridTemplate>;
	for (auto&& view : world.Query<TUpdatedQuery>())
	{
		const auto& gridTemplate = view.ReadRequired<shared::tilemap::GridTemplate>();
		auto& gridComponent = view.WriteRequired<shared::tilemap::GridComponent>();
		gridComponent.m_GridSize = gridTemplate.m_GridSize;
		gridComponent.m_TileSize = gridTemplate.m_TileSize;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const shared::tilemap::GridTemplate>>())
	{
		world.RemoveComponent<shared::tilemap::GridComponent>(view);
	}
}

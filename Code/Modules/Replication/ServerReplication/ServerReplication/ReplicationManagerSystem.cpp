#include "ReplicationPCH.h"
#include "ServerReplication/ReplicationManagerSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/ReplicationComponent.h"
#include "ECS/WorldView.h"
#include "SharedCursor/CursorTransformComponent.h"
#include "SharedVoxel/VoxelChunkComponent.h"

void server::replication::ManagerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Set<ecs::Entity> entitiesToUpdate;
	entitiesToUpdate.Add(world.Query<ecs::query::Added<shared::cursor::TransformComponent>>());
	entitiesToUpdate.Add(world.Query<ecs::query::Removed<shared::cursor::TransformComponent>>());
	entitiesToUpdate.Add(world.Query<ecs::query::Added<shared::voxel::ChunkComponent>>());
	entitiesToUpdate.Add(world.Query<ecs::query::Removed<shared::voxel::ChunkComponent>>());

	for (const ecs::Entity& entity : entitiesToUpdate)
	{
		const bool hasComponent = world.HasComponent<ecs::ReplicationComponent>(entity);
		const bool wantComponent = 
			world.HasComponent<shared::cursor::TransformComponent>(entity) ||
			world.HasComponent<shared::voxel::ChunkComponent>(entity);
		if (!hasComponent && wantComponent)
		{
			world.AddComponent<ecs::ReplicationComponent>(entity);
		}
		else if (hasComponent && !wantComponent)
		{
			world.RemoveComponent<ecs::ReplicationComponent>(entity);
		}
	}
}
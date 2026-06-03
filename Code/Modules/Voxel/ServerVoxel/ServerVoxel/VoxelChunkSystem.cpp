#include "VoxelPCH.h"
#include "ServerVoxel/VoxelChunkSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/TransformComponent.h"
#include "Math/VectorMath.h"
#include "SharedVoxel/VoxelChunkComponent.h"
#include "SharedVoxel/VoxelChunkTemplate.h"
#include "SharedVoxel/VoxelClientModifyEvent.h"

namespace
{
	using World = server::voxel::ChunkSystem::World;

	int32 ToInnerIndex(const Vector3i& innerPos)
	{
		constexpr int32 s_Count1D = shared::voxel::s_BlockCount1D;
		constexpr int32 s_Count2D = shared::voxel::s_BlockCount2D;
		return innerPos.x
			+ innerPos.y * s_Count1D
			+ innerPos.z * s_Count2D;
	}

	void ProcessTemplates(World& world)
	{
		using TAddedQuery = ecs::query
			::Added<const shared::voxel::ChunkTemplate>
			::Include<const shared::voxel::ChunkTemplate>;
		for (auto&& view : world.Query<TAddedQuery>())
		{
			const auto& chunkTemplate = view.ReadRequired<shared::voxel::ChunkTemplate>();
			auto& chunkComponent = world.AddComponent<shared::voxel::ChunkComponent>(view);
			chunkComponent.m_Data = chunkTemplate.m_Data;
		}

		using TUpdatedQuery = ecs::query
			::Updated<
			const shared::voxel::ChunkTemplate>
			::Include<
			const shared::voxel::ChunkComponent,
			const shared::voxel::ChunkTemplate>;
		for (auto&& view : world.Query<TUpdatedQuery>())
		{
			const auto& chunkTemplate = view.ReadRequired<shared::voxel::ChunkTemplate>();
			auto& chunkComponent = view.WriteRequired<shared::voxel::ChunkComponent>();
			chunkComponent.m_Data = chunkTemplate.m_Data;
		}

		for (auto&& view : world.Query<ecs::query::Removed<const shared::voxel::ChunkTemplate>>())
		{
			world.RemoveComponent<shared::voxel::ChunkComponent>(view);
		}
	}

	void ProcessRequests(World& world)
	{
		for (const auto& event : world.Events<const shared::voxel::ClientModifyEvent>())
		{
			for (const shared::voxel::Modify& change : event.m_Changes)
			{
				for (auto&& chunkView : world.Query<ecs::query::Include<shared::voxel::ChunkComponent, const eng::TransformComponent>>())
				{
					// #todo: better way to detect if position is inside or not
					const auto& transform = chunkView.ReadRequired<eng::TransformComponent>();
					const Vector3i changePos = math::ToGridPos(change.m_WorldPos - transform.m_Translate, shared::voxel::s_ChunkSize1D);
					if (changePos != Vector3i::Zero)
						continue;

					const Vector3f worldPos = change.m_WorldPos - transform.m_Translate;
					const Vector3i innerPos = math::ToGridPos(worldPos, shared::voxel::s_BlockSize1D);
					const int32 innerIndex = ToInnerIndex(innerPos);
					auto& chunk = chunkView.WriteRequired<shared::voxel::ChunkComponent>();
					chunk.m_Data[innerIndex] = change.m_Data;
				}
			}
		}
	}
}

void server::voxel::ChunkSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ProcessTemplates(world);
	ProcessRequests(world);
}

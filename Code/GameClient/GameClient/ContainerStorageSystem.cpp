#include "GameClientPCH.h"
#include "GameClient/ContainerStorageSystem.h"

#include <Core/Algorithms.h>
#include <Core/Nullable.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "GameClient/ContainerComponents.h"

namespace
{
	using World = container::StorageSystem::World;

	container::EError VerifyStorageCreate(World& world, const ecs::Entity& entity, const container::FrameData& frameData)
	{
		const auto& requestComponent = world.GetComponent<const container::StorageCreateRequestComponent>(entity);
		if (world.HasComponent<container::OwnerComponent>(requestComponent.m_Owner))
		{
			const auto& ownerComponent = world.GetComponent<const container::OwnerComponent>(requestComponent.m_Owner);
			if (ownerComponent.m_Storages.Contains(requestComponent.m_Type))
				return container::EError::StorageDuplicate;
		}

		auto hasDuplicate = [&](const container::StorageData& rhs)
		{
			return requestComponent.m_Owner == rhs.m_Owner
				&& requestComponent.m_Type == rhs.m_Type;
		};

		if (core::ContainsIf(frameData.m_StorageCreated, hasDuplicate))
			return container::EError::StorageDuplicate;

		return container::EError::None;
	}

	using DestroyWorld = container::StorageSystem::World;
	container::EError VerifyStorageDestroy(DestroyWorld& world, const ecs::Entity& entity, const container::FrameData& frameData)
	{
		const auto& requestComponent = world.GetComponent<const container::StorageDestroyRequestComponent>(entity);
		if (requestComponent.m_Storage.IsUnassigned())
			return container::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return container::EError::StorageDead;
		if (!world.HasComponent<container::StorageComponent>(requestComponent.m_Storage))
			return container::EError::StorageMissing;

		const auto& storageComponent = world.GetComponent<const container::StorageComponent>(requestComponent.m_Storage);
		auto hasDuplicate = [&](const container::StorageData& rhs)
		{
			return storageComponent.m_Owner == rhs.m_Owner
				&& storageComponent.m_Type == rhs.m_Type;
		};

		if (core::ContainsIf(frameData.m_StorageDestroyed, hasDuplicate))
			return container::EError::StorageDead;

		return container::EError::None;
	}
}

void container::StorageSystem::Update(World& world, const GameTime& gameTime)
{
	FrameData frameData;
	ProcessMemberChanges(world, frameData);
	ProcessStorageRequests(world, frameData);
	ProcessOwnerRequests(world, frameData);
}

void container::StorageSystem::ProcessMemberChanges(World& world, FrameData& frameData)
{
	const auto& memberChangesComponent = world.GetSingleton<const container::MemberChangesComponent>();
	for (const MemberChange& data : memberChangesComponent.m_Added)
	{
		auto& storageComponent = world.GetComponent<container::StorageComponent>(data.m_Storage);
		storageComponent.m_Members.Add(data.m_Member);
	}

	for (const MemberChange& data : memberChangesComponent.m_Removed)
	{
		auto& storageComponent = world.GetComponent<container::StorageComponent>(data.m_Storage);
		storageComponent.m_Members.Remove(data.m_Member);
	}
}

void container::StorageSystem::ProcessStorageRequests(World& world, FrameData& frameData)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const container::StorageCreateRequestComponent>>())
	{
		const EError error = VerifyStorageCreate(world, entity, frameData);
		const auto& requestComponent = world.GetComponent<const container::StorageCreateRequestComponent>(entity);

		auto& resultComponent = world.AddComponent<container::StorageCreateResultComponent>(entity);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const ecs::Entity storageEntity = world.CreateEntity();
			auto& storageComponent = world.AddComponent<container::StorageComponent>(storageEntity);
			storageComponent.m_Owner = requestComponent.m_Owner;
			storageComponent.m_Limit = requestComponent.m_Limit;
			storageComponent.m_Type = requestComponent.m_Type;

			resultComponent.m_Storage = storageEntity;

			// update frame data so we can detect duplicates
			StorageData& createData = frameData.m_StorageCreated.Emplace();
			createData.m_Storage = storageEntity;
			createData.m_Owner = requestComponent.m_Owner;
			createData.m_Type = requestComponent.m_Type;
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const container::StorageDestroyRequestComponent>>())
	{
		const EError error = VerifyStorageDestroy(world, entity, frameData);
		const auto& requestComponent = world.GetComponent<const container::StorageDestroyRequestComponent>(entity);

		if (error == EError::None)
		{
			world.DestroyEntity(requestComponent.m_Storage);

			// update frame data so we can detect duplicates
			const auto& storageComponent = world.GetComponent<const container::StorageComponent>(requestComponent.m_Storage);
			StorageData& destroyData = frameData.m_StorageDestroyed.Emplace();
			destroyData.m_Storage = requestComponent.m_Storage;
			destroyData.m_Owner = storageComponent.m_Owner;
			destroyData.m_Type = storageComponent.m_Type;
		}

		auto& resultComponent = world.AddComponent<container::StorageDestroyResultComponent>(entity);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Storage = requestComponent.m_Storage;
		resultComponent.m_Error = error;
	}

	// cleanup results on the next frame
	for (const ecs::Entity& entity : world.Query<ecs::query::Include<container::StorageCreateResultComponent>>())
		world.RemoveComponent<container::StorageCreateResultComponent>(entity);
	for (const ecs::Entity& entity : world.Query<ecs::query::Include<container::StorageDestroyResultComponent>>())
		world.RemoveComponent<container::StorageDestroyResultComponent>(entity);
}

void container::StorageSystem::ProcessOwnerRequests(World& world, FrameData& frameData)
{
	Map<ecs::Entity, Array<const StorageData*>> requests;
	for (const StorageData& data : frameData.m_StorageCreated)
		requests[data.m_Owner].Append(&data);

	for (auto&& [ownerEntity, createRequests] : requests)
	{
		if (ownerEntity.IsUnassigned())
			continue;

		auto& ownerComponent = world.HasComponent<container::OwnerComponent>(ownerEntity)
			? world.GetComponent<container::OwnerComponent>(ownerEntity)
			: world.AddComponent<container::OwnerComponent>(ownerEntity);

		for (const StorageData* data : createRequests)
			ownerComponent.m_Storages.Set(data->m_Type, data->m_Storage);
	}

	for (const StorageData& data : frameData.m_StorageDestroyed)
	{
		if (data.m_Owner.IsUnassigned())
			continue;

		auto& ownerComponent = world.GetComponent<container::OwnerComponent>(data.m_Owner);
		ownerComponent.m_Storages.Remove(data.m_Type);

		if (ownerComponent.m_Storages.IsEmpty())
			world.RemoveComponent<container::OwnerComponent>(data.m_Owner);
	}
}
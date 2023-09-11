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

	container::EError VerifyStorageCreate(World& world, const ecs::Entity& entity, const container::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.GetComponent<const container::StorageCreateRequestComponent>(entity);
		if (world.HasComponent<container::OwnerComponent>(requestComponent.m_Owner))
		{
			const auto& ownerComponent = world.GetComponent<const container::OwnerComponent>(requestComponent.m_Owner);
			if (ownerComponent.m_Storages.Contains(requestComponent.m_Type))
				return container::EError::StorageDuplicate;
		}

		auto hasDuplicate = [&](const container::StorageChange& rhs)
		{
			return requestComponent.m_Owner == rhs.m_Owner
				&& requestComponent.m_Type == rhs.m_Type;
		};

		if (core::ContainsIf(frameData.m_Created, hasDuplicate))
			return container::EError::StorageDuplicate;

		return container::EError::None;
	}

	using DestroyWorld = container::StorageSystem::World;
	container::EError VerifyStorageDestroy(DestroyWorld& world, const ecs::Entity& entity, const container::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.GetComponent<const container::StorageDestroyRequestComponent>(entity);
		if (requestComponent.m_Storage.IsUnassigned())
			return container::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return container::EError::StorageDead;
		if (!world.HasComponent<container::StorageComponent>(requestComponent.m_Storage))
			return container::EError::StorageMissing;

		const auto& storageComponent = world.GetComponent<const container::StorageComponent>(requestComponent.m_Storage);
		auto hasDuplicate = [&](const container::StorageChange& rhs)
		{
			return storageComponent.m_Owner == rhs.m_Owner
				&& storageComponent.m_Type == rhs.m_Type;
		};

		if (core::ContainsIf(frameData.m_Destroyed, hasDuplicate))
			return container::EError::StorageDead;

		return container::EError::None;
	}
}

void container::StorageSystem::Initialise(World& world)
{
	world.AddSingleton<container::StorageChangesComponent>();
}

void container::StorageSystem::Shutdown(World& world)
{
	world.RemoveSingleton<container::StorageChangesComponent>();
}

void container::StorageSystem::Update(World& world, const GameTime& gameTime)
{
	ProcessMemberChanges(world);
	ProcessStorageRequests(world);
}

void container::StorageSystem::ProcessMemberChanges(World& world)
{
	const auto& memberChangesComponent = world.GetSingleton<const container::MemberChangesComponent>();
	for (const MemberAdded& data : memberChangesComponent.m_Added)
	{
		auto& storageComponent = world.GetComponent<container::StorageComponent>(data.m_Storage);
		storageComponent.m_Members.Add(data.m_Member);
	}

	for (const MemberRemoved& data : memberChangesComponent.m_Removed)
	{
		auto& storageComponent = world.GetComponent<container::StorageComponent>(data.m_Storage);
		storageComponent.m_Members.Remove(data.m_Member);
	}

	for (const MemberMoved& data : memberChangesComponent.m_Moved)
	{
		auto& storageAComponent = world.GetComponent<container::StorageComponent>(data.m_StorageA);
		auto& storageBComponent = world.GetComponent<container::StorageComponent>(data.m_StorageB);
		storageAComponent.m_Members.Remove(data.m_Member);
		storageBComponent.m_Members.Add(data.m_Member);
	}

	// member lifetime is external to the container systems
	for (const ecs::Entity& memberEntity : world.Query<ecs::query::Removed<const container::MemberComponent>>())
	{
		const auto& memberComponent = world.GetComponent<const container::MemberComponent>(memberEntity, false);
		
		// if storage was also destroyed in the previous frame
		if (!world.IsAlive(memberComponent.m_Storage))
			continue;

		auto& storageComponent = world.GetComponent<container::StorageComponent>(memberComponent.m_Storage);
		storageComponent.m_Members.Remove(memberEntity);
	}
}

void container::StorageSystem::ProcessStorageRequests(World& world)
{
	auto& storageChangesComponent = world.GetSingleton<container::StorageChangesComponent>();
	storageChangesComponent.m_Created.RemoveAll();
	storageChangesComponent.m_Destroyed.RemoveAll();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const container::StorageCreateRequestComponent>>())
	{
		const EError error = VerifyStorageCreate(world, entity, storageChangesComponent);
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

			// add to frame data so we can detect duplicates
			StorageChange& createData = storageChangesComponent.m_Created.Emplace();
			createData.m_Storage = storageEntity;
			createData.m_Owner = requestComponent.m_Owner;
			createData.m_Type = requestComponent.m_Type;
		}
	}

	// #todo: destroy storage when owner is destroyed?

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const container::StorageDestroyRequestComponent>>())
	{
		const EError error = VerifyStorageDestroy(world, entity, storageChangesComponent);
		const auto& requestComponent = world.GetComponent<const container::StorageDestroyRequestComponent>(entity);

		if (error == EError::None)
		{
			world.DestroyEntity(requestComponent.m_Storage);

			// add to frame data so we can detect duplicates
			const auto& storageComponent = world.GetComponent<const container::StorageComponent>(requestComponent.m_Storage);
			StorageChange& destroyData = storageChangesComponent.m_Destroyed.Emplace();
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
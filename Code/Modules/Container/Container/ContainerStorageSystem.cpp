#include "ContainerPCH.h"
#include "Container/ContainerStorageSystem.h"

#include "Container/ContainerMemberAddRequestComponent.h"
#include "Container/ContainerMemberAddResultComponent.h"
#include "Container/ContainerMemberComponent.h"
#include "Container/ContainerMemberMoveRequestComponent.h"
#include "Container/ContainerMemberMoveResultComponent.h"
#include "Container/ContainerMemberRemoveRequestComponent.h"
#include "Container/ContainerMemberRemoveResultComponent.h"
#include "Container/ContainerOwnerComponent.h"
#include "Container/ContainerStorageChangesSingleton.h"
#include "Container/ContainerStorageComponent.h"
#include "Container/ContainerStorageCreateRequestComponent.h"
#include "Container/ContainerStorageCreateResultComponent.h"
#include "Container/ContainerStorageDestroyRequestComponent.h"
#include "Container/ContainerStorageDestroyResultComponent.h"
#include "Core/Algorithms.h"
#include "Core/Optional.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"

namespace
{
	using World = container::StorageSystem::World;

	container::EError VerifyMemberAdd(World& world, const ecs::Entity& entity, const container::StorageChangesSingleton& frameData)
	{
		const auto& requestComponent = world.ReadComponent<container::MemberAddRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return container::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return container::EError::MemberDead;
		if (world.HasComponent<container::MemberComponent>(requestComponent.m_Member))
			return container::EError::MemberDuplicate;

		if (requestComponent.m_Storage.IsUnassigned())
			return container::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return container::EError::StorageDead;
		if (!world.HasComponent<container::StorageComponent>(requestComponent.m_Storage))
			return container::EError::StorageMissing;

		auto hasDuplicate = [&](const container::MemberAdded& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberAdded, hasDuplicate))
			return container::EError::MemberDuplicate;

		return container::EError::None;
	}

	container::EError VerifyMemberMove(World& world, const ecs::Entity& entity, const container::StorageChangesSingleton& frameData)
	{
		const auto& requestComponent = world.ReadComponent<container::MemberMoveRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return container::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return container::EError::MemberDead;
		if (!world.HasComponent<container::MemberComponent>(requestComponent.m_Member))
			return container::EError::MemberMissing;

		const auto& memberComponent = world.ReadComponent<container::MemberComponent>(requestComponent.m_Member);
		if (!world.IsAlive(memberComponent.m_Storage))
			return container::EError::StorageDead;
		if (!world.IsAlive(requestComponent.m_Storage))
			return container::EError::StorageDead;

		auto wasRemoved = [&](const container::MemberRemoved& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberRemoved, wasRemoved))
			return container::EError::MemberDead;

		return container::EError::None;
	}

	container::EError VerifyMemberRemove(World& world, const ecs::Entity& entity, const container::StorageChangesSingleton& frameData)
	{
		const auto& requestComponent = world.ReadComponent<container::MemberRemoveRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return container::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return container::EError::MemberDead;
		if (!world.HasComponent<container::MemberComponent>(requestComponent.m_Member))
			return container::EError::MemberMissing;

		const auto& memberComponent = world.ReadComponent<container::MemberComponent>(requestComponent.m_Member);
		if (!world.IsAlive(memberComponent.m_Storage))
			return container::EError::StorageDead;

		auto hasDuplicate = [&](const container::MemberRemoved& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberRemoved, hasDuplicate))
			return container::EError::MemberDead;

		return container::EError::None;
	}

	container::EError VerifyStorageCreate(World& world, const ecs::Entity& entity, const container::StorageChangesSingleton& frameData)
	{
		const auto& requestComponent = world.ReadComponent<container::StorageCreateRequestComponent>(entity);
		if (world.HasComponent<container::OwnerComponent>(requestComponent.m_Owner))
		{
			const auto& ownerComponent = world.ReadComponent<container::OwnerComponent>(requestComponent.m_Owner);
			if (ownerComponent.m_Storages.Contains(requestComponent.m_Type))
				return container::EError::StorageDuplicate;
		}

		auto hasDuplicate = [&](const container::StorageChange& rhs)
		{
			return requestComponent.m_Owner == rhs.m_Owner
				&& requestComponent.m_Type == rhs.m_Type;
		};

		if (enumerate::ContainsIf(frameData.m_StorageCreated, hasDuplicate))
			return container::EError::StorageDuplicate;

		return container::EError::None;
	}

	using DestroyWorld = container::StorageSystem::World;
	container::EError VerifyStorageDestroy(DestroyWorld& world, const ecs::Entity& entity, const container::StorageChangesSingleton& frameData)
	{
		const auto& requestComponent = world.ReadComponent<container::StorageDestroyRequestComponent>(entity);
		if (requestComponent.m_Storage.IsUnassigned())
			return container::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return container::EError::StorageDead;
		if (!world.HasComponent<container::StorageComponent>(requestComponent.m_Storage))
			return container::EError::StorageMissing;

		const auto& storageComponent = world.ReadComponent<container::StorageComponent>(requestComponent.m_Storage);
		auto hasDuplicate = [&](const container::StorageChange& rhs)
		{
			return storageComponent.m_Owner == rhs.m_Owner
				&& storageComponent.m_Type == rhs.m_Type;
		};

		if (enumerate::ContainsIf(frameData.m_StorageDestroyed, hasDuplicate))
			return container::EError::StorageDead;

		return container::EError::None;
	}
}

void container::StorageSystem::Update(World& world, const GameTime& gameTime)
{
	// clear values from previous frame
	auto& changesComponent = world.WriteSingleton<container::StorageChangesSingleton>();
	changesComponent.m_MemberAdded.RemoveAll();
	changesComponent.m_MemberMoved.RemoveAll();
	changesComponent.m_MemberRemoved.RemoveAll();
	changesComponent.m_StorageCreated.RemoveAll();
	changesComponent.m_StorageDestroyed.RemoveAll();

	ProcessMemberAddRequests(world);
	ProcessMemberMoveRequests(world);
	ProcessMemberRemoveRequests(world);
	ProcessStorageRequests(world);

	// cleanup results on the next frame
	for (auto&& view : world.Query<ecs::query::Include<container::MemberAddResultComponent>>())
		world.RemoveComponent<container::MemberAddResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<container::MemberMoveResultComponent>>())
		world.RemoveComponent<container::MemberMoveResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<container::MemberRemoveResultComponent>>())
		world.RemoveComponent<container::MemberRemoveResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<container::StorageCreateResultComponent>>())
		world.RemoveComponent<container::StorageCreateResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<container::StorageDestroyResultComponent>>())
		world.RemoveComponent<container::StorageDestroyResultComponent>(view);
}

void container::StorageSystem::ProcessMemberAddRequests(World& world)
{
	auto& changesComponent = world.WriteSingleton<container::StorageChangesSingleton>();
	for (const ecs::Entity& requestEntity : world.Query<ecs::query::Added<const container::MemberAddRequestComponent>>())
	{
		const EError error = VerifyMemberAdd(world, requestEntity, changesComponent);

		const auto& requestComponent = world.ReadComponent<container::MemberAddRequestComponent>(requestEntity);
		auto& resultComponent = world.AddComponent<container::MemberAddResultComponent>(requestEntity);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			auto& storageComponent = world.WriteComponent<container::StorageComponent>(requestComponent.m_Storage);
			storageComponent.m_Members.Add(requestComponent.m_Member);

			MemberAdded& data = changesComponent.m_MemberAdded.Emplace();
			data.m_Storage = requestComponent.m_Storage;
			data.m_Member = requestComponent.m_Member;
			data.m_Count = requestComponent.m_Count;
			data.m_GridX = requestComponent.m_GridX;
			data.m_GridY = requestComponent.m_GridY;
			data.m_Type = requestComponent.m_Type;
		}
	}
}

void container::StorageSystem::ProcessMemberMoveRequests(World& world)
{
	auto& changesComponent = world.WriteSingleton<container::StorageChangesSingleton>();
	for (const ecs::Entity& requestEntity : world.Query<ecs::query::Added<const container::MemberMoveRequestComponent>>())
	{
		const EError error = VerifyMemberMove(world, requestEntity, changesComponent);

		const auto& requestComponent = world.ReadComponent<container::MemberMoveRequestComponent>(requestEntity);
		auto& resultComponent = world.AddComponent<container::MemberMoveResultComponent>(requestEntity);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const auto& memberComponent = world.ReadComponent<container::MemberComponent>(requestComponent.m_Member);
			auto& storageAComponent = world.WriteComponent<container::StorageComponent>(memberComponent.m_Storage);
			auto& storageBComponent = world.WriteComponent<container::StorageComponent>(requestComponent.m_Storage);
			storageAComponent.m_Members.Remove(requestComponent.m_Member);
			storageBComponent.m_Members.Add(requestComponent.m_Member);

			MemberMoved& data = changesComponent.m_MemberMoved.Emplace();
			data.m_Storage = requestComponent.m_Storage;
			data.m_Member = requestComponent.m_Member;
		}
	}
}

void container::StorageSystem::ProcessMemberRemoveRequests(World& world)
{
	auto& changesComponent = world.WriteSingleton<container::StorageChangesSingleton>();
	for (const ecs::Entity& requestEntity : world.Query<ecs::query::Added<const container::MemberRemoveRequestComponent>>())
	{
		const EError error = VerifyMemberRemove(world, requestEntity, changesComponent);

		const auto& requestComponent = world.ReadComponent<container::MemberRemoveRequestComponent>(requestEntity);
		auto& resultComponent = world.AddComponent<container::MemberRemoveResultComponent>(requestEntity);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const auto& memberComponent = world.ReadComponent<container::MemberComponent>(requestComponent.m_Member);
			auto& storageComponent = world.WriteComponent<container::StorageComponent>(memberComponent.m_Storage);
			storageComponent.m_Members.Remove(requestComponent.m_Member);

			MemberRemoved& data = changesComponent.m_MemberRemoved.Emplace();
			data.m_Storage = memberComponent.m_Storage;
			data.m_Member = requestComponent.m_Member;
		}
	}

	// #todo: only dead entities
	// member lifetime is external so we need to listen to it being destroyed
	for (const ecs::Entity& memberEntity : world.Query<ecs::query::Removed<const container::MemberComponent>>())
	{
		// if storage was also destroyed in the previous frame
		const auto& memberComponent = world.ReadComponent<container::MemberComponent>(memberEntity, false);
		if (!world.IsAlive(memberComponent.m_Storage))
			continue;

		auto& storageComponent = world.WriteComponent<container::StorageComponent>(memberComponent.m_Storage);
		storageComponent.m_Members.Remove(memberEntity);
	}
}

void container::StorageSystem::ProcessStorageRequests(World& world)
{
	auto& storageChangesComponent = world.WriteSingleton<container::StorageChangesSingleton>();

	using CreateQuery = ecs::query
		::Added<const container::StorageCreateRequestComponent>
		::Include<const container::StorageCreateRequestComponent>;
	for (auto&& view : world.Query<CreateQuery>())
	{
		const EError error = VerifyStorageCreate(world, view, storageChangesComponent);
		const auto& requestComponent = view.ReadRequired<container::StorageCreateRequestComponent>();

		auto& resultComponent = world.AddComponent<container::StorageCreateResultComponent>(view);
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
			StorageChange& createData = storageChangesComponent.m_StorageCreated.Emplace();
			createData.m_Storage = storageEntity;
			createData.m_Owner = requestComponent.m_Owner;
			createData.m_Type = requestComponent.m_Type;
		}
	}

	// #todo: destroy storage when owner is destroyed?
	using DestroyQuery = ecs::query
		::Added<const container::StorageDestroyRequestComponent>
		::Include<const container::StorageDestroyRequestComponent>;
	for (auto&& view : world.Query<DestroyQuery>())
	{
		const EError error = VerifyStorageDestroy(world, view, storageChangesComponent);
		const auto& requestComponent = view.ReadRequired<container::StorageDestroyRequestComponent>();

		if (error == EError::None)
		{
			world.DestroyEntity(requestComponent.m_Storage);

			// add to frame data so we can detect duplicates
			const auto& storageComponent = world.ReadComponent<container::StorageComponent>(requestComponent.m_Storage);
			StorageChange& destroyData = storageChangesComponent.m_StorageDestroyed.Emplace();
			destroyData.m_Storage = requestComponent.m_Storage;
			destroyData.m_Owner = storageComponent.m_Owner;
			destroyData.m_Type = storageComponent.m_Type;
		}

		auto& resultComponent = world.AddComponent<container::StorageDestroyResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Storage = requestComponent.m_Storage;
		resultComponent.m_Error = error;
	}
}
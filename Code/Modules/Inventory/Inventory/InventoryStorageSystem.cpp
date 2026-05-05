#include "InventoryPCH.h"
#include "Inventory/InventoryStorageSystem.h"

#include "Core/Algorithms.h"
#include "Core/Optional.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Inventory/InventoryMemberAddRequestComponent.h"
#include "Inventory/InventoryMemberAddResultComponent.h"
#include "Inventory/InventoryMemberComponent.h"
#include "Inventory/InventoryMemberMoveRequestComponent.h"
#include "Inventory/InventoryMemberMoveResultComponent.h"
#include "Inventory/InventoryMemberRemoveRequestComponent.h"
#include "Inventory/InventoryMemberRemoveResultComponent.h"
#include "Inventory/InventoryOwnerComponent.h"
#include "Inventory/InventoryStorageChangesComponent.h"
#include "Inventory/InventoryStorageComponent.h"
#include "Inventory/InventoryStorageCreateRequestComponent.h"
#include "Inventory/InventoryStorageCreateResultComponent.h"
#include "Inventory/InventoryStorageDestroyRequestComponent.h"
#include "Inventory/InventoryStorageDestroyResultComponent.h"

namespace
{
	using World = inventory::StorageSystem::World;

	inventory::EError VerifyMemberAdd(World& world, const ecs::Entity& entity, const inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<inventory::MemberAddRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return inventory::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return inventory::EError::MemberDead;
		if (world.HasComponent<inventory::MemberComponent>(requestComponent.m_Member))
			return inventory::EError::MemberDuplicate;

		if (requestComponent.m_Storage.IsUnassigned())
			return inventory::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return inventory::EError::StorageDead;
		if (!world.HasComponent<inventory::StorageComponent>(requestComponent.m_Storage))
			return inventory::EError::StorageMissing;

		auto hasDuplicate = [&](const inventory::MemberAdded& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberAdded, hasDuplicate))
			return inventory::EError::MemberDuplicate;

		return inventory::EError::None;
	}

	inventory::EError VerifyMemberMove(World& world, const ecs::Entity& entity, const inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<inventory::MemberMoveRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return inventory::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return inventory::EError::MemberDead;
		if (!world.HasComponent<inventory::MemberComponent>(requestComponent.m_Member))
			return inventory::EError::MemberMissing;

		const auto& memberComponent = world.ReadComponent<inventory::MemberComponent>(requestComponent.m_Member);
		if (!world.IsAlive(memberComponent.m_Storage))
			return inventory::EError::StorageDead;
		if (!world.IsAlive(requestComponent.m_Storage))
			return inventory::EError::StorageDead;

		auto wasRemoved = [&](const inventory::MemberRemoved& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberRemoved, wasRemoved))
			return inventory::EError::MemberDead;

		return inventory::EError::None;
	}

	inventory::EError VerifyMemberRemove(World& world, const ecs::Entity& entity, const inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<inventory::MemberRemoveRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return inventory::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return inventory::EError::MemberDead;
		if (!world.HasComponent<inventory::MemberComponent>(requestComponent.m_Member))
			return inventory::EError::MemberMissing;

		const auto& memberComponent = world.ReadComponent<inventory::MemberComponent>(requestComponent.m_Member);
		if (!world.IsAlive(memberComponent.m_Storage))
			return inventory::EError::StorageDead;

		auto hasDuplicate = [&](const inventory::MemberRemoved& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberRemoved, hasDuplicate))
			return inventory::EError::MemberDead;

		return inventory::EError::None;
	}

	inventory::EError VerifyStorageCreate(World& world, const ecs::Entity& entity, const inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<inventory::StorageCreateRequestComponent>(entity);
		if (world.HasComponent<inventory::OwnerComponent>(requestComponent.m_Owner))
		{
			const auto& ownerComponent = world.ReadComponent<inventory::OwnerComponent>(requestComponent.m_Owner);
			if (ownerComponent.m_Storages.Contains(requestComponent.m_Type))
				return inventory::EError::StorageDuplicate;
		}

		auto hasDuplicate = [&](const inventory::StorageChange& rhs)
		{
			return requestComponent.m_Owner == rhs.m_Owner
				&& requestComponent.m_Type == rhs.m_Type;
		};

		if (enumerate::ContainsIf(frameData.m_StorageCreated, hasDuplicate))
			return inventory::EError::StorageDuplicate;

		return inventory::EError::None;
	}

	using DestroyWorld = inventory::StorageSystem::World;
	inventory::EError VerifyStorageDestroy(DestroyWorld& world, const ecs::Entity& entity, const inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<inventory::StorageDestroyRequestComponent>(entity);
		if (requestComponent.m_Storage.IsUnassigned())
			return inventory::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return inventory::EError::StorageDead;
		if (!world.HasComponent<inventory::StorageComponent>(requestComponent.m_Storage))
			return inventory::EError::StorageMissing;

		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(requestComponent.m_Storage);
		auto hasDuplicate = [&](const inventory::StorageChange& rhs)
		{
			return storageComponent.m_Owner == rhs.m_Owner
				&& storageComponent.m_Type == rhs.m_Type;
		};

		if (enumerate::ContainsIf(frameData.m_StorageDestroyed, hasDuplicate))
			return inventory::EError::StorageDead;

		return inventory::EError::None;
	}
}

void inventory::StorageSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// clear values from previous frame
	auto& changesComponent = world.WriteComponent<inventory::StorageChangesComponent>();
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
	for (auto&& view : world.Query<ecs::query::Include<const inventory::MemberAddResultComponent>>())
		world.RemoveComponent<inventory::MemberAddResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<const inventory::MemberMoveResultComponent>>())
		world.RemoveComponent<inventory::MemberMoveResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<const inventory::MemberRemoveResultComponent>>())
		world.RemoveComponent<inventory::MemberRemoveResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<const inventory::StorageCreateResultComponent>>())
		world.RemoveComponent<inventory::StorageCreateResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<const inventory::StorageDestroyResultComponent>>())
		world.RemoveComponent<inventory::StorageDestroyResultComponent>(view);
}

void inventory::StorageSystem::ProcessMemberAddRequests(World& world)
{
	auto& changesComponent = world.WriteComponent<inventory::StorageChangesComponent>();
	
	using AddedQuery = ecs::query
		::Added<const inventory::MemberAddRequestComponent>
		::Include<const inventory::MemberAddRequestComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const EError error = VerifyMemberAdd(world, view, changesComponent);

		const auto& requestComponent = view.ReadRequired<inventory::MemberAddRequestComponent>();
		auto& resultComponent = world.AddComponent<inventory::MemberAddResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			auto& storageComponent = world.WriteComponent<inventory::StorageComponent>(requestComponent.m_Storage);
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

void inventory::StorageSystem::ProcessMemberMoveRequests(World& world)
{
	auto& changesComponent = world.WriteComponent<inventory::StorageChangesComponent>();
	
	using AddedQuery = ecs::query
		::Added<const inventory::MemberMoveRequestComponent>
		::Include<const inventory::MemberMoveRequestComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const EError error = VerifyMemberMove(world, view, changesComponent);

		const auto& requestComponent = view.ReadRequired<inventory::MemberMoveRequestComponent>();
		auto& resultComponent = world.AddComponent<inventory::MemberMoveResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const auto& memberComponent = world.ReadComponent<inventory::MemberComponent>(requestComponent.m_Member);
			auto& storageAComponent = world.WriteComponent<inventory::StorageComponent>(memberComponent.m_Storage);
			auto& storageBComponent = world.WriteComponent<inventory::StorageComponent>(requestComponent.m_Storage);
			storageAComponent.m_Members.Remove(requestComponent.m_Member);
			storageBComponent.m_Members.Add(requestComponent.m_Member);

			MemberMoved& data = changesComponent.m_MemberMoved.Emplace();
			data.m_Storage = requestComponent.m_Storage;
			data.m_Member = requestComponent.m_Member;
		}
	}
}

void inventory::StorageSystem::ProcessMemberRemoveRequests(World& world)
{
	auto& changesComponent = world.WriteComponent<inventory::StorageChangesComponent>();
	
	using AddedQuery = ecs::query
		::Added<const inventory::MemberRemoveRequestComponent>
		::Include<const inventory::MemberRemoveRequestComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const EError error = VerifyMemberRemove(world, view, changesComponent);

		const auto& requestComponent = view.ReadRequired<inventory::MemberRemoveRequestComponent>();
		auto& resultComponent = world.AddComponent<inventory::MemberRemoveResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const auto& memberComponent = world.ReadComponent<inventory::MemberComponent>(requestComponent.m_Member);
			auto& storageComponent = world.WriteComponent<inventory::StorageComponent>(memberComponent.m_Storage);
			storageComponent.m_Members.Remove(requestComponent.m_Member);

			MemberRemoved& data = changesComponent.m_MemberRemoved.Emplace();
			data.m_Storage = memberComponent.m_Storage;
			data.m_Member = requestComponent.m_Member;
		}
	}

	// member lifetime is external so we need to listen to it being destroyed
	using RemovedQuery = ecs::query
		::Condition<ecs::Dead>
		::Removed<inventory::MemberComponent>;
	for (auto&& view : world.Query<RemovedQuery>())
	{
		// if storage was also destroyed in the previous frame
		const auto& memberComponent = world.ReadComponent<inventory::MemberComponent>(view, false);
		if (!world.IsAlive(memberComponent.m_Storage))
			continue;

		auto& storageComponent = world.WriteComponent<inventory::StorageComponent>(memberComponent.m_Storage);
		storageComponent.m_Members.Remove(view);
	}
}

void inventory::StorageSystem::ProcessStorageRequests(World& world)
{
	auto& storageChangesComponent = world.WriteComponent<inventory::StorageChangesComponent>();

	using CreateQuery = ecs::query
		::Added<const inventory::StorageCreateRequestComponent>
		::Include<const inventory::StorageCreateRequestComponent>;
	for (auto&& view : world.Query<CreateQuery>())
	{
		const EError error = VerifyStorageCreate(world, view, storageChangesComponent);
		const auto& requestComponent = view.ReadRequired<inventory::StorageCreateRequestComponent>();

		auto& resultComponent = world.AddComponent<inventory::StorageCreateResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const ecs::Entity storageEntity = world.CreateEntity();
			auto& storageComponent = world.AddComponent<inventory::StorageComponent>(storageEntity);
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
		::Added<const inventory::StorageDestroyRequestComponent>
		::Include<const inventory::StorageDestroyRequestComponent>;
	for (auto&& view : world.Query<DestroyQuery>())
	{
		const EError error = VerifyStorageDestroy(world, view, storageChangesComponent);
		const auto& requestComponent = view.ReadRequired<inventory::StorageDestroyRequestComponent>();

		if (error == EError::None)
		{
			world.DestroyEntity(requestComponent.m_Storage);

			// add to frame data so we can detect duplicates
			const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(requestComponent.m_Storage);
			StorageChange& destroyData = storageChangesComponent.m_StorageDestroyed.Emplace();
			destroyData.m_Storage = requestComponent.m_Storage;
			destroyData.m_Owner = storageComponent.m_Owner;
			destroyData.m_Type = storageComponent.m_Type;
		}

		auto& resultComponent = world.AddComponent<inventory::StorageDestroyResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Storage = requestComponent.m_Storage;
		resultComponent.m_Error = error;
	}
}
#include "InventoryPCH.h"
#include "SharedInventory/InventoryStorageSystem.h"

#include "Core/Algorithms.h"
#include "Core/Optional.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "SharedInventory/InventoryMemberAddRequestComponent.h"
#include "SharedInventory/InventoryMemberAddResultComponent.h"
#include "SharedInventory/InventoryMemberComponent.h"
#include "SharedInventory/InventoryMemberMoveRequestComponent.h"
#include "SharedInventory/InventoryMemberMoveResultComponent.h"
#include "SharedInventory/InventoryMemberRemoveRequestComponent.h"
#include "SharedInventory/InventoryMemberRemoveResultComponent.h"
#include "SharedInventory/InventoryOwnerComponent.h"
#include "SharedInventory/InventoryStorageChangesComponent.h"
#include "SharedInventory/InventoryStorageComponent.h"
#include "SharedInventory/InventoryStorageCreateRequestComponent.h"
#include "SharedInventory/InventoryStorageCreateResultComponent.h"
#include "SharedInventory/InventoryStorageDestroyRequestComponent.h"
#include "SharedInventory/InventoryStorageDestroyResultComponent.h"

namespace
{
	using World = shared::inventory::StorageSystem::World;

	shared::inventory::EError VerifyMemberAdd(World& world, const ecs::Entity& entity, const shared::inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<shared::inventory::MemberAddRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return shared::inventory::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return shared::inventory::EError::MemberDead;
		if (world.HasComponent<shared::inventory::MemberComponent>(requestComponent.m_Member))
			return shared::inventory::EError::MemberDuplicate;

		if (requestComponent.m_Storage.IsUnassigned())
			return shared::inventory::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return shared::inventory::EError::StorageDead;
		if (!world.HasComponent<shared::inventory::StorageComponent>(requestComponent.m_Storage))
			return shared::inventory::EError::StorageMissing;

		auto hasDuplicate = [&](const shared::inventory::MemberAdded& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberAdded, hasDuplicate))
			return shared::inventory::EError::MemberDuplicate;

		return shared::inventory::EError::None;
	}

	shared::inventory::EError VerifyMemberMove(World& world, const ecs::Entity& entity, const shared::inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<shared::inventory::MemberMoveRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return shared::inventory::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return shared::inventory::EError::MemberDead;
		if (!world.HasComponent<shared::inventory::MemberComponent>(requestComponent.m_Member))
			return shared::inventory::EError::MemberMissing;

		const auto& memberComponent = world.ReadComponent<shared::inventory::MemberComponent>(requestComponent.m_Member);
		if (!world.IsAlive(memberComponent.m_Storage))
			return shared::inventory::EError::StorageDead;
		if (!world.IsAlive(requestComponent.m_Storage))
			return shared::inventory::EError::StorageDead;

		auto wasRemoved = [&](const shared::inventory::MemberRemoved& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberRemoved, wasRemoved))
			return shared::inventory::EError::MemberDead;

		return shared::inventory::EError::None;
	}

	shared::inventory::EError VerifyMemberRemove(World& world, const ecs::Entity& entity, const shared::inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<shared::inventory::MemberRemoveRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return shared::inventory::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return shared::inventory::EError::MemberDead;
		if (!world.HasComponent<shared::inventory::MemberComponent>(requestComponent.m_Member))
			return shared::inventory::EError::MemberMissing;

		const auto& memberComponent = world.ReadComponent<shared::inventory::MemberComponent>(requestComponent.m_Member);
		if (!world.IsAlive(memberComponent.m_Storage))
			return shared::inventory::EError::StorageDead;

		auto hasDuplicate = [&](const shared::inventory::MemberRemoved& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberRemoved, hasDuplicate))
			return shared::inventory::EError::MemberDead;

		return shared::inventory::EError::None;
	}

	shared::inventory::EError VerifyStorageCreate(World& world, const ecs::Entity& entity, const shared::inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<shared::inventory::StorageCreateRequestComponent>(entity);
		if (world.HasComponent<shared::inventory::OwnerComponent>(requestComponent.m_Owner))
		{
			const auto& ownerComponent = world.ReadComponent<shared::inventory::OwnerComponent>(requestComponent.m_Owner);
			if (ownerComponent.m_Storages.Contains(requestComponent.m_Type))
				return shared::inventory::EError::StorageDuplicate;
		}

		auto hasDuplicate = [&](const shared::inventory::StorageChange& rhs)
		{
			return requestComponent.m_Owner == rhs.m_Owner
				&& requestComponent.m_Type == rhs.m_Type;
		};

		if (enumerate::ContainsIf(frameData.m_StorageCreated, hasDuplicate))
			return shared::inventory::EError::StorageDuplicate;

		return shared::inventory::EError::None;
	}

	using DestroyWorld = shared::inventory::StorageSystem::World;
	shared::inventory::EError VerifyStorageDestroy(DestroyWorld& world, const ecs::Entity& entity, const shared::inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<shared::inventory::StorageDestroyRequestComponent>(entity);
		if (requestComponent.m_Storage.IsUnassigned())
			return shared::inventory::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return shared::inventory::EError::StorageDead;
		if (!world.HasComponent<shared::inventory::StorageComponent>(requestComponent.m_Storage))
			return shared::inventory::EError::StorageMissing;

		const auto& storageComponent = world.ReadComponent<shared::inventory::StorageComponent>(requestComponent.m_Storage);
		auto hasDuplicate = [&](const shared::inventory::StorageChange& rhs)
		{
			return storageComponent.m_Owner == rhs.m_Owner
				&& storageComponent.m_Type == rhs.m_Type;
		};

		if (enumerate::ContainsIf(frameData.m_StorageDestroyed, hasDuplicate))
			return shared::inventory::EError::StorageDead;

		return shared::inventory::EError::None;
	}
}

void shared::inventory::StorageSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// clear values from previous frame
	auto& changesComponent = world.WriteComponent<shared::inventory::StorageChangesComponent>();
	changesComponent.m_MemberAdded.RemoveAll();
	changesComponent.m_MemberMoved.RemoveAll();
	changesComponent.m_MemberRemoved.RemoveAll();
	changesComponent.m_StorageCreated.RemoveAll();
	changesComponent.m_StorageDestroyed.RemoveAll();

	ProcessMemberAddRequests(world);
	ProcessMemberMoveRequests(world);
	ProcessMemberRemoveRequests(world);
	ProcessStorageRequests(world);
}

void shared::inventory::StorageSystem::ProcessMemberAddRequests(World& world)
{
	auto& changesComponent = world.WriteComponent<shared::inventory::StorageChangesComponent>();

	using AddedQuery = ecs::query
		::Added<const shared::inventory::MemberAddRequestComponent>
		::Include<const shared::inventory::MemberAddRequestComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const EError error = VerifyMemberAdd(world, view, changesComponent);

		const auto& requestComponent = view.ReadRequired<shared::inventory::MemberAddRequestComponent>();
		auto& resultComponent = world.AddComponent<shared::inventory::MemberAddResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			auto& storageComponent = world.WriteComponent<shared::inventory::StorageComponent>(requestComponent.m_Storage);
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

void shared::inventory::StorageSystem::ProcessMemberMoveRequests(World& world)
{
	auto& changesComponent = world.WriteComponent<shared::inventory::StorageChangesComponent>();

	using AddedQuery = ecs::query
		::Added<const shared::inventory::MemberMoveRequestComponent>
		::Include<const shared::inventory::MemberMoveRequestComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const EError error = VerifyMemberMove(world, view, changesComponent);

		const auto& requestComponent = view.ReadRequired<shared::inventory::MemberMoveRequestComponent>();
		auto& resultComponent = world.AddComponent<shared::inventory::MemberMoveResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const auto& memberComponent = world.ReadComponent<shared::inventory::MemberComponent>(requestComponent.m_Member);
			auto& storageAComponent = world.WriteComponent<shared::inventory::StorageComponent>(memberComponent.m_Storage);
			auto& storageBComponent = world.WriteComponent<shared::inventory::StorageComponent>(requestComponent.m_Storage);
			storageAComponent.m_Members.Remove(requestComponent.m_Member);
			storageBComponent.m_Members.Add(requestComponent.m_Member);

			MemberMoved& data = changesComponent.m_MemberMoved.Emplace();
			data.m_Storage = requestComponent.m_Storage;
			data.m_Member = requestComponent.m_Member;
		}
	}
}

void shared::inventory::StorageSystem::ProcessMemberRemoveRequests(World& world)
{
	auto& changesComponent = world.WriteComponent<shared::inventory::StorageChangesComponent>();

	using AddedQuery = ecs::query
		::Added<const shared::inventory::MemberRemoveRequestComponent>
		::Include<const shared::inventory::MemberRemoveRequestComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const EError error = VerifyMemberRemove(world, view, changesComponent);

		const auto& requestComponent = view.ReadRequired<shared::inventory::MemberRemoveRequestComponent>();
		auto& resultComponent = world.AddComponent<shared::inventory::MemberRemoveResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const auto& memberComponent = world.ReadComponent<shared::inventory::MemberComponent>(requestComponent.m_Member);
			auto& storageComponent = world.WriteComponent<shared::inventory::StorageComponent>(memberComponent.m_Storage);
			storageComponent.m_Members.Remove(requestComponent.m_Member);

			MemberRemoved& data = changesComponent.m_MemberRemoved.Emplace();
			data.m_Storage = memberComponent.m_Storage;
			data.m_Member = requestComponent.m_Member;
		}
	}

	// member lifetime is external so we need to listen to it being destroyed
	using RemovedQuery = ecs::query
		::Condition<ecs::Dead>
		::Removed<shared::inventory::MemberComponent>;
	for (auto&& view : world.Query<RemovedQuery>())
	{
		// if storage was also destroyed in the previous frame
		const auto& memberComponent = world.ReadComponent<shared::inventory::MemberComponent>(view, false);
		if (!world.IsAlive(memberComponent.m_Storage))
			continue;

		auto& storageComponent = world.WriteComponent<shared::inventory::StorageComponent>(memberComponent.m_Storage);
		storageComponent.m_Members.Remove(view);
	}
}

void shared::inventory::StorageSystem::ProcessStorageRequests(World& world)
{
	auto& storageChangesComponent = world.WriteComponent<shared::inventory::StorageChangesComponent>();

	using CreateQuery = ecs::query
		::Added<const shared::inventory::StorageCreateRequestComponent>
		::Include<const shared::inventory::StorageCreateRequestComponent>;
	for (auto&& view : world.Query<CreateQuery>())
	{
		const EError error = VerifyStorageCreate(world, view, storageChangesComponent);
		const auto& requestComponent = view.ReadRequired<shared::inventory::StorageCreateRequestComponent>();

		auto& resultComponent = world.AddComponent<shared::inventory::StorageCreateResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const ecs::Entity storageEntity = world.CreateEntity();
			auto& storageComponent = world.AddComponent<shared::inventory::StorageComponent>(storageEntity);
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
		::Added<const shared::inventory::StorageDestroyRequestComponent>
		::Include<const shared::inventory::StorageDestroyRequestComponent>;
	for (auto&& view : world.Query<DestroyQuery>())
	{
		const EError error = VerifyStorageDestroy(world, view, storageChangesComponent);
		const auto& requestComponent = view.ReadRequired<shared::inventory::StorageDestroyRequestComponent>();

		if (error == EError::None)
		{
			world.DestroyEntity(requestComponent.m_Storage);

			// add to frame data so we can detect duplicates
			const auto& storageComponent = world.ReadComponent<shared::inventory::StorageComponent>(requestComponent.m_Storage);
			StorageChange& destroyData = storageChangesComponent.m_StorageDestroyed.Emplace();
			destroyData.m_Storage = requestComponent.m_Storage;
			destroyData.m_Owner = storageComponent.m_Owner;
			destroyData.m_Type = storageComponent.m_Type;
		}

		auto& resultComponent = world.AddComponent<shared::inventory::StorageDestroyResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Storage = requestComponent.m_Storage;
		resultComponent.m_Error = error;
	}
}
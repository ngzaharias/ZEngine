#include "ServerInventoryPCH.h"
#include "ServerInventory/InventoryStorageSystem.h"

#include "Core/Algorithms.h"
#include "Core/Optional.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "ServerInventory/InventoryMemberAddRequestComponent.h"
#include "ServerInventory/InventoryMemberAddResultComponent.h"
#include "ServerInventory/InventoryMemberComponent.h"
#include "ServerInventory/InventoryMemberMoveRequestComponent.h"
#include "ServerInventory/InventoryMemberMoveResultComponent.h"
#include "ServerInventory/InventoryMemberRemoveRequestComponent.h"
#include "ServerInventory/InventoryMemberRemoveResultComponent.h"
#include "ServerInventory/InventoryOwnerComponent.h"
#include "ServerInventory/InventoryStorageChangesComponent.h"
#include "ServerInventory/InventoryStorageComponent.h"
#include "ServerInventory/InventoryStorageCreateRequestComponent.h"
#include "ServerInventory/InventoryStorageCreateResultComponent.h"
#include "ServerInventory/InventoryStorageDestroyRequestComponent.h"
#include "ServerInventory/InventoryStorageDestroyResultComponent.h"

namespace
{
	using World = server::inventory::StorageSystem::World;

	server::inventory::EError VerifyMemberAdd(World& world, const ecs::Entity& entity, const server::inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<server::inventory::MemberAddRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return server::inventory::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return server::inventory::EError::MemberDead;
		if (world.HasComponent<server::inventory::MemberComponent>(requestComponent.m_Member))
			return server::inventory::EError::MemberDuplicate;

		if (requestComponent.m_Storage.IsUnassigned())
			return server::inventory::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return server::inventory::EError::StorageDead;
		if (!world.HasComponent<server::inventory::StorageComponent>(requestComponent.m_Storage))
			return server::inventory::EError::StorageMissing;

		auto hasDuplicate = [&](const server::inventory::MemberAdded& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberAdded, hasDuplicate))
			return server::inventory::EError::MemberDuplicate;

		return server::inventory::EError::None;
	}

	server::inventory::EError VerifyMemberMove(World& world, const ecs::Entity& entity, const server::inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<server::inventory::MemberMoveRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return server::inventory::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return server::inventory::EError::MemberDead;
		if (!world.HasComponent<server::inventory::MemberComponent>(requestComponent.m_Member))
			return server::inventory::EError::MemberMissing;

		const auto& memberComponent = world.ReadComponent<server::inventory::MemberComponent>(requestComponent.m_Member);
		if (!world.IsAlive(memberComponent.m_Storage))
			return server::inventory::EError::StorageDead;
		if (!world.IsAlive(requestComponent.m_Storage))
			return server::inventory::EError::StorageDead;

		auto wasRemoved = [&](const server::inventory::MemberRemoved& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberRemoved, wasRemoved))
			return server::inventory::EError::MemberDead;

		return server::inventory::EError::None;
	}

	server::inventory::EError VerifyMemberRemove(World& world, const ecs::Entity& entity, const server::inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<server::inventory::MemberRemoveRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return server::inventory::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return server::inventory::EError::MemberDead;
		if (!world.HasComponent<server::inventory::MemberComponent>(requestComponent.m_Member))
			return server::inventory::EError::MemberMissing;

		const auto& memberComponent = world.ReadComponent<server::inventory::MemberComponent>(requestComponent.m_Member);
		if (!world.IsAlive(memberComponent.m_Storage))
			return server::inventory::EError::StorageDead;

		auto hasDuplicate = [&](const server::inventory::MemberRemoved& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (enumerate::ContainsIf(frameData.m_MemberRemoved, hasDuplicate))
			return server::inventory::EError::MemberDead;

		return server::inventory::EError::None;
	}

	server::inventory::EError VerifyStorageCreate(World& world, const ecs::Entity& entity, const server::inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<server::inventory::StorageCreateRequestComponent>(entity);
		if (world.HasComponent<server::inventory::OwnerComponent>(requestComponent.m_Owner))
		{
			const auto& ownerComponent = world.ReadComponent<server::inventory::OwnerComponent>(requestComponent.m_Owner);
			if (ownerComponent.m_Storages.Contains(requestComponent.m_Type))
				return server::inventory::EError::StorageDuplicate;
		}

		auto hasDuplicate = [&](const server::inventory::StorageChange& rhs)
		{
			return requestComponent.m_Owner == rhs.m_Owner
				&& requestComponent.m_Type == rhs.m_Type;
		};

		if (enumerate::ContainsIf(frameData.m_StorageCreated, hasDuplicate))
			return server::inventory::EError::StorageDuplicate;

		return server::inventory::EError::None;
	}

	using DestroyWorld = server::inventory::StorageSystem::World;
	server::inventory::EError VerifyStorageDestroy(DestroyWorld& world, const ecs::Entity& entity, const server::inventory::StorageChangesComponent& frameData)
	{
		const auto& requestComponent = world.ReadComponent<server::inventory::StorageDestroyRequestComponent>(entity);
		if (requestComponent.m_Storage.IsUnassigned())
			return server::inventory::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return server::inventory::EError::StorageDead;
		if (!world.HasComponent<server::inventory::StorageComponent>(requestComponent.m_Storage))
			return server::inventory::EError::StorageMissing;

		const auto& storageComponent = world.ReadComponent<server::inventory::StorageComponent>(requestComponent.m_Storage);
		auto hasDuplicate = [&](const server::inventory::StorageChange& rhs)
		{
			return storageComponent.m_Owner == rhs.m_Owner
				&& storageComponent.m_Type == rhs.m_Type;
		};

		if (enumerate::ContainsIf(frameData.m_StorageDestroyed, hasDuplicate))
			return server::inventory::EError::StorageDead;

		return server::inventory::EError::None;
	}
}

void server::inventory::StorageSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// clear values from previous frame
	auto& changesComponent = world.WriteComponent<server::inventory::StorageChangesComponent>();
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
	for (auto&& view : world.Query<ecs::query::Include<const server::inventory::MemberAddResultComponent>>())
		world.RemoveComponent<server::inventory::MemberAddResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<const server::inventory::MemberMoveResultComponent>>())
		world.RemoveComponent<server::inventory::MemberMoveResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<const server::inventory::MemberRemoveResultComponent>>())
		world.RemoveComponent<server::inventory::MemberRemoveResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<const server::inventory::StorageCreateResultComponent>>())
		world.RemoveComponent<server::inventory::StorageCreateResultComponent>(view);
	for (auto&& view : world.Query<ecs::query::Include<const server::inventory::StorageDestroyResultComponent>>())
		world.RemoveComponent<server::inventory::StorageDestroyResultComponent>(view);
}

void server::inventory::StorageSystem::ProcessMemberAddRequests(World& world)
{
	auto& changesComponent = world.WriteComponent<server::inventory::StorageChangesComponent>();
	
	using AddedQuery = ecs::query
		::Added<const server::inventory::MemberAddRequestComponent>
		::Include<const server::inventory::MemberAddRequestComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const EError error = VerifyMemberAdd(world, view, changesComponent);

		const auto& requestComponent = view.ReadRequired<server::inventory::MemberAddRequestComponent>();
		auto& resultComponent = world.AddComponent<server::inventory::MemberAddResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			auto& storageComponent = world.WriteComponent<server::inventory::StorageComponent>(requestComponent.m_Storage);
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

void server::inventory::StorageSystem::ProcessMemberMoveRequests(World& world)
{
	auto& changesComponent = world.WriteComponent<server::inventory::StorageChangesComponent>();
	
	using AddedQuery = ecs::query
		::Added<const server::inventory::MemberMoveRequestComponent>
		::Include<const server::inventory::MemberMoveRequestComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const EError error = VerifyMemberMove(world, view, changesComponent);

		const auto& requestComponent = view.ReadRequired<server::inventory::MemberMoveRequestComponent>();
		auto& resultComponent = world.AddComponent<server::inventory::MemberMoveResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const auto& memberComponent = world.ReadComponent<server::inventory::MemberComponent>(requestComponent.m_Member);
			auto& storageAComponent = world.WriteComponent<server::inventory::StorageComponent>(memberComponent.m_Storage);
			auto& storageBComponent = world.WriteComponent<server::inventory::StorageComponent>(requestComponent.m_Storage);
			storageAComponent.m_Members.Remove(requestComponent.m_Member);
			storageBComponent.m_Members.Add(requestComponent.m_Member);

			MemberMoved& data = changesComponent.m_MemberMoved.Emplace();
			data.m_Storage = requestComponent.m_Storage;
			data.m_Member = requestComponent.m_Member;
		}
	}
}

void server::inventory::StorageSystem::ProcessMemberRemoveRequests(World& world)
{
	auto& changesComponent = world.WriteComponent<server::inventory::StorageChangesComponent>();
	
	using AddedQuery = ecs::query
		::Added<const server::inventory::MemberRemoveRequestComponent>
		::Include<const server::inventory::MemberRemoveRequestComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const EError error = VerifyMemberRemove(world, view, changesComponent);

		const auto& requestComponent = view.ReadRequired<server::inventory::MemberRemoveRequestComponent>();
		auto& resultComponent = world.AddComponent<server::inventory::MemberRemoveResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const auto& memberComponent = world.ReadComponent<server::inventory::MemberComponent>(requestComponent.m_Member);
			auto& storageComponent = world.WriteComponent<server::inventory::StorageComponent>(memberComponent.m_Storage);
			storageComponent.m_Members.Remove(requestComponent.m_Member);

			MemberRemoved& data = changesComponent.m_MemberRemoved.Emplace();
			data.m_Storage = memberComponent.m_Storage;
			data.m_Member = requestComponent.m_Member;
		}
	}

	// #todo: only dead entities
	// member lifetime is external so we need to listen to it being destroyed
	for (auto&& view : world.Query<ecs::query::Removed<const server::inventory::MemberComponent>>())
	{
		// if storage was also destroyed in the previous frame
		const auto& memberComponent = world.ReadComponent<server::inventory::MemberComponent>(view, false);
		if (!world.IsAlive(memberComponent.m_Storage))
			continue;

		auto& storageComponent = world.WriteComponent<server::inventory::StorageComponent>(memberComponent.m_Storage);
		storageComponent.m_Members.Remove(view);
	}
}

void server::inventory::StorageSystem::ProcessStorageRequests(World& world)
{
	auto& storageChangesComponent = world.WriteComponent<server::inventory::StorageChangesComponent>();

	using CreateQuery = ecs::query
		::Added<const server::inventory::StorageCreateRequestComponent>
		::Include<const server::inventory::StorageCreateRequestComponent>;
	for (auto&& view : world.Query<CreateQuery>())
	{
		const EError error = VerifyStorageCreate(world, view, storageChangesComponent);
		const auto& requestComponent = view.ReadRequired<server::inventory::StorageCreateRequestComponent>();

		auto& resultComponent = world.AddComponent<server::inventory::StorageCreateResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const ecs::Entity storageEntity = world.CreateEntity();
			auto& storageComponent = world.AddComponent<server::inventory::StorageComponent>(storageEntity);
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
		::Added<const server::inventory::StorageDestroyRequestComponent>
		::Include<const server::inventory::StorageDestroyRequestComponent>;
	for (auto&& view : world.Query<DestroyQuery>())
	{
		const EError error = VerifyStorageDestroy(world, view, storageChangesComponent);
		const auto& requestComponent = view.ReadRequired<server::inventory::StorageDestroyRequestComponent>();

		if (error == EError::None)
		{
			world.DestroyEntity(requestComponent.m_Storage);

			// add to frame data so we can detect duplicates
			const auto& storageComponent = world.ReadComponent<server::inventory::StorageComponent>(requestComponent.m_Storage);
			StorageChange& destroyData = storageChangesComponent.m_StorageDestroyed.Emplace();
			destroyData.m_Storage = requestComponent.m_Storage;
			destroyData.m_Owner = storageComponent.m_Owner;
			destroyData.m_Type = storageComponent.m_Type;
		}

		auto& resultComponent = world.AddComponent<server::inventory::StorageDestroyResultComponent>(view);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Storage = requestComponent.m_Storage;
		resultComponent.m_Error = error;
	}
}
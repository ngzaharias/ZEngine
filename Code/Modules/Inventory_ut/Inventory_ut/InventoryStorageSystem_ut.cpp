#include <Catch2/catch.hpp>

#include "Core/GameTime.h"
#include "Core/Types.h"
#include "ECS/UTHelpers.h"
#include "ECS/WorldView.h"
#include "Inventory/InventoryMemberAddRequestComponent.h"
#include "Inventory/InventoryMemberAddResultComponent.h"
#include "Inventory/InventoryMemberComponent.h"
#include "Inventory/InventoryMemberMoveRequestComponent.h"
#include "Inventory/InventoryMemberMoveResultComponent.h"
#include "Inventory/InventoryMemberRemoveRequestComponent.h"
#include "Inventory/InventoryMemberRemoveResultComponent.h"
#include "Inventory/InventoryMemberSystem.h"
#include "Inventory/InventoryOwnerComponent.h"
#include "Inventory/InventoryOwnerSystem.h"
#include "Inventory/InventoryStorageChangesComponent.h"
#include "Inventory/InventoryStorageComponent.h"
#include "Inventory/InventoryStorageCreateRequestComponent.h"
#include "Inventory/InventoryStorageCreateResultComponent.h"
#include "Inventory/InventoryStorageDestroyRequestComponent.h"
#include "Inventory/InventoryStorageDestroyResultComponent.h"
#include "Inventory/InventoryStorageSystem.h"

namespace
{
	struct RAIIHelper : public ut::RAIIHelper
	{
		RAIIHelper()
		{
			m_EntityWorld.RegisterComponent<inventory::MemberAddRequestComponent>();
			m_EntityWorld.RegisterComponent<inventory::MemberAddResultComponent>();
			m_EntityWorld.RegisterComponent<inventory::MemberComponent>();
			m_EntityWorld.RegisterComponent<inventory::MemberMoveRequestComponent>();
			m_EntityWorld.RegisterComponent<inventory::MemberMoveResultComponent>();
			m_EntityWorld.RegisterComponent<inventory::MemberRemoveRequestComponent>();
			m_EntityWorld.RegisterComponent<inventory::MemberRemoveResultComponent>();
			m_EntityWorld.RegisterComponent<inventory::OwnerComponent>();
			m_EntityWorld.RegisterComponent<inventory::StorageComponent>();
			m_EntityWorld.RegisterComponent<inventory::StorageCreateRequestComponent>();
			m_EntityWorld.RegisterComponent<inventory::StorageCreateResultComponent>();
			m_EntityWorld.RegisterComponent<inventory::StorageDestroyRequestComponent>();
			m_EntityWorld.RegisterComponent<inventory::StorageDestroyResultComponent>();
			m_EntityWorld.RegisterComponent<inventory::StorageChangesComponent>();
			m_EntityWorld.RegisterSystem<inventory::StorageSystem>();
			m_EntityWorld.RegisterSystem<inventory::MemberSystem>();
			m_EntityWorld.RegisterSystem<inventory::OwnerSystem>();

			Initialise();
		}
	};

	ecs::Entity CreateMember(RAIIHelper& world, const ecs::Entity& storageEntity)
	{
		const ecs::Entity memberEntity = world.CreateEntity();
		const ecs::Entity requestEntity = world.CreateEntity();
		world.Update();

		auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestEntity);
		requestComponent.m_Member = memberEntity;
		requestComponent.m_Storage = storageEntity;
		world.Update(2);

		return memberEntity;
	}

	ecs::Entity CreateStorage(RAIIHelper& world, const ecs::Entity& ownerEntity, const int32 type)
	{
		const ecs::Entity requestEntity = world.CreateEntity();
		world.Update();

		auto& requestComponent = world.AddComponent<inventory::StorageCreateRequestComponent>(requestEntity);
		requestComponent.m_Owner = ownerEntity;
		requestComponent.m_Type = type;
		world.Update(2);

		const auto& resultComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestEntity);
		return resultComponent.m_Storage;
	}

	void DestroyStorage(RAIIHelper& world, const ecs::Entity& storageEntity)
	{
		const ecs::Entity requestEntity = world.CreateEntity();
		auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestEntity);
		requestComponent.m_Storage = storageEntity;
		world.Update(2);
	}
}

CATCH_REGISTER_ENUM(inventory::EError, 
	inventory::EError::None,
	inventory::EError::MemberDead,
	inventory::EError::MemberDuplicate,
	inventory::EError::MemberMissing,
	inventory::EError::MemberUnassigned,
	inventory::EError::OwnerDead,
	inventory::EError::OwnerDestroyed,
	inventory::EError::OwnerInvalid,
	inventory::EError::OwnerUnassigned,
	inventory::EError::StorageDead,
	inventory::EError::StorageDuplicate,
	inventory::EError::StorageMissing,
	inventory::EError::StorageUnassigned);

TEST_CASE("inventory::StorageSystem::Member Add")
{
	RAIIHelper world;

	const str::Guid transactionId = str::Guid::Generate();
	const ecs::Entity deadEntity = world.CreateEntity();
	const ecs::Entity memberEntity = world.CreateEntity();
	const ecs::Entity requestAEntity = world.CreateEntity();
	const ecs::Entity requestBEntity = world.CreateEntity();
	const ecs::Entity storageEntity = CreateStorage(world, ecs::Entity::Unassigned, 0);
	world.Update();
	world.DestroyEntity(deadEntity);
	world.Update();

	SECTION("Member is Unassigned Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = ecs::Entity::Unassigned;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == ecs::Entity::Unassigned);
		CHECK(resultComponent.m_Error == inventory::EError::MemberUnassigned);
	}

	SECTION("Member is Dead Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = deadEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == deadEntity);
		CHECK(resultComponent.m_Error == inventory::EError::MemberDead);
	}

	SECTION("Member already in Storage")
	{
		// make a member
		world.AddComponent<inventory::MemberComponent>(memberEntity);
		world.Update();

		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == inventory::EError::MemberDuplicate);
	}

	SECTION("Storage is Unassigned Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = ecs::Entity::Unassigned;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == inventory::EError::StorageUnassigned);
	}

	SECTION("Storage is Dead Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = deadEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == inventory::EError::StorageDead);
	}

	SECTION("Storage Destroyed in Previous Frame")
	{
		// remove storage
		world.RemoveComponent<inventory::StorageComponent>(storageEntity);
		world.Update();

		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == inventory::EError::StorageMissing);
	}

	SECTION("Duplicate Request in Previous Frame")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// check result a
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestAEntity));
		const auto& resultAComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Member == memberEntity);
		CHECK(resultAComponent.m_Error == inventory::EError::None);

		world.Update();

		// check result b
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestBEntity));
		const auto& resultBComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Member == memberEntity);
		CHECK(resultBComponent.m_Error == inventory::EError::MemberDuplicate);
	}

	SECTION("Duplicate Request in Same Frame")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result a
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestAEntity));
		const auto& resultAComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Member == memberEntity);
		CHECK(resultAComponent.m_Error == inventory::EError::None);

		// check result b
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestBEntity));
		const auto& resultBComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Member == memberEntity);
		CHECK(resultBComponent.m_Error == inventory::EError::MemberDuplicate);
	}

	SECTION("Regular Flow")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Count = 1337;
			requestComponent.m_GridX = 123;
			requestComponent.m_GridY = 456;
			requestComponent.m_Type = 666;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == inventory::EError::None);

		// check member
		REQUIRE(world.HasComponent<inventory::MemberComponent>(resultComponent.m_Member));
		const auto& memberComponent = world.ReadComponent<inventory::MemberComponent>(resultComponent.m_Member);
		CHECK(memberComponent.m_Storage == storageEntity);
		CHECK(memberComponent.m_Count == 1337);
		CHECK(memberComponent.m_GridX == 123);
		CHECK(memberComponent.m_GridY == 456);
		CHECK(memberComponent.m_Type == 666);

		// checks changes
		const auto& changesComponent = world.m_EntityWorld.ReadComponent<inventory::StorageChangesComponent>();
		REQUIRE(changesComponent.m_MemberAdded.GetCount() == 1);
		CHECK(changesComponent.m_MemberAdded[0].m_Member == memberEntity);
		CHECK(changesComponent.m_MemberAdded[0].m_Storage == storageEntity);
	}

	// always verify that all changes are cleared on the next frame
	{
		world.Update();

		const auto& changesComponent = world.m_EntityWorld.ReadComponent<inventory::StorageChangesComponent>();
		CHECK(changesComponent.m_MemberAdded.GetCount() == 0);
		CHECK(changesComponent.m_MemberMoved.GetCount() == 0);
		CHECK(changesComponent.m_MemberRemoved.GetCount() == 0);
	}
}

TEST_CASE("inventory::StorageSystem::Member Remove")
{
	RAIIHelper world;

	const str::Guid transactionId = str::Guid::Generate();
	const ecs::Entity deadEntity = world.CreateEntity();
	const ecs::Entity requestAEntity = world.CreateEntity();
	const ecs::Entity requestBEntity = world.CreateEntity();
	const ecs::Entity storageEntity = CreateStorage(world, ecs::Entity::Unassigned, 0);
	const ecs::Entity memberEntity = CreateMember(world, storageEntity);
	world.Update();
	world.DestroyEntity(deadEntity);
	world.Update();

	{
		REQUIRE(world.HasComponent<inventory::MemberComponent>(memberEntity));
		REQUIRE(world.HasComponent<inventory::StorageComponent>(storageEntity));
		const auto& memberComponent = world.ReadComponent<inventory::MemberComponent>(memberEntity);
		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(storageEntity);
		REQUIRE(storageComponent.m_Members.Contains(memberEntity));
	}

	SECTION("Member is Unassigned Entity")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = ecs::Entity::Unassigned;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == ecs::Entity::Unassigned);
		CHECK(resultComponent.m_Error == inventory::EError::MemberUnassigned);

		// check storage
		REQUIRE(world.HasComponent<inventory::StorageComponent>(storageEntity));
		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(storageEntity);
		CHECK(storageComponent.m_Members.Contains(memberEntity));
	}

	SECTION("Member is Dead Entity")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = deadEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == deadEntity);
		CHECK(resultComponent.m_Error == inventory::EError::MemberDead);

		// check storage
		REQUIRE(world.HasComponent<inventory::StorageComponent>(storageEntity));
		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(storageEntity);
		CHECK(storageComponent.m_Members.Contains(memberEntity));
	}

	SECTION("Member was Destroyed in Previous Frame")
	{
		// remove member
		world.DestroyEntity(memberEntity);
		world.Update();

		// request remove
		{
			auto& requestComponent = world.AddComponent<inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == inventory::EError::MemberDead);

		// check storage
		REQUIRE(world.HasComponent<inventory::StorageComponent>(storageEntity));
		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(storageEntity);
		CHECK(!storageComponent.m_Members.Contains(memberEntity));
	}

	SECTION("Storage was Destroyed in Previous Frame")
	{
		// destroy the storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// request remove
		{
			auto& requestComponent = world.AddComponent<inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == inventory::EError::MemberMissing);

		// check storage
		CHECK(!world.HasComponent<inventory::StorageComponent>(storageEntity));
	}

	SECTION("Storage was Destroyed in Same Frame")
	{
		// destroy the storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageEntity;
		}

		// request remove
		{
			auto& requestComponent = world.AddComponent<inventory::MemberRemoveRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberRemoveResultComponent>(requestBEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberRemoveResultComponent>(requestBEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == inventory::EError::None);

		// check storage
		CHECK(!world.HasComponent<inventory::StorageComponent>(storageEntity));
	}

	SECTION("Duplicate Request in Previous Frame")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update();

		// request remove
		{
			auto& requestComponent = world.AddComponent<inventory::MemberRemoveRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update();

		// check result a
		REQUIRE(world.HasComponent<inventory::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultAComponent = world.ReadComponent<inventory::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Member == memberEntity);
		CHECK(resultAComponent.m_Error == inventory::EError::None);

		world.Update();

		// check result b
		REQUIRE(world.HasComponent<inventory::MemberRemoveResultComponent>(requestBEntity));
		const auto& resultBComponent = world.ReadComponent<inventory::MemberRemoveResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Member == memberEntity);
		CHECK(resultBComponent.m_Error == inventory::EError::MemberMissing);

		// check storage
		REQUIRE(world.HasComponent<inventory::StorageComponent>(storageEntity));
		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(storageEntity);
		CHECK(!storageComponent.m_Members.Contains(memberEntity));
	}

	SECTION("Duplicate Request in Same Frame")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		// request remove
		{
			auto& requestComponent = world.AddComponent<inventory::MemberRemoveRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);

		// check result a
		REQUIRE(world.HasComponent<inventory::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultAComponent = world.ReadComponent<inventory::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Member == memberEntity);
		CHECK(resultAComponent.m_Error == inventory::EError::None);

		// check result b
		REQUIRE(world.HasComponent<inventory::MemberRemoveResultComponent>(requestBEntity));
		const auto& resultBComponent = world.ReadComponent<inventory::MemberRemoveResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Member == memberEntity);
		CHECK(resultBComponent.m_Error == inventory::EError::MemberDead);

		// check storage
		REQUIRE(world.HasComponent<inventory::StorageComponent>(storageEntity));
		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(storageEntity);
		CHECK(!storageComponent.m_Members.Contains(memberEntity));
	}

	SECTION("Member and Storage are Destroyed in Same Frame")
	{
		// destroy the storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageEntity;
		}

		world.DestroyEntity(memberEntity);
		world.Update(2);

		// no crash
	}

	SECTION("Regular Flow")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == inventory::EError::None);

		// check storage
		REQUIRE(world.HasComponent<inventory::StorageComponent>(storageEntity));
		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(storageEntity);
		CHECK(!storageComponent.m_Members.Contains(memberEntity));

		// check changes
		const auto& changesComponent = world.m_EntityWorld.ReadComponent<inventory::StorageChangesComponent>();
		REQUIRE(changesComponent.m_MemberRemoved.GetCount() == 1);
		CHECK(changesComponent.m_MemberRemoved[0].m_Member == memberEntity);
		CHECK(changesComponent.m_MemberRemoved[0].m_Storage == storageEntity);
	}

	// always verify that all changes are cleared on the next frame
	{
		world.Update();

		const auto& changesComponent = world.m_EntityWorld.ReadComponent<inventory::StorageChangesComponent>();
		CHECK(changesComponent.m_MemberAdded.GetCount() == 0);
		CHECK(changesComponent.m_MemberMoved.GetCount() == 0);
		CHECK(changesComponent.m_MemberRemoved.GetCount() == 0);
	}
}

TEST_CASE("inventory::StorageSystem::Storage Create")
{
	RAIIHelper world;

	const str::Guid transactionId = str::Guid::Generate();
	const ecs::Entity ownerEntity = world.CreateEntity();
	const ecs::Entity requestAEntity = world.CreateEntity();
	const ecs::Entity requestBEntity = world.CreateEntity();
	world.Update();

	SECTION("Failure: Duplicate Storage, Same Frame")
	{
		// request create
		{
			auto& requestComponent = world.AddComponent<inventory::StorageCreateRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Limit = 1337;
			requestComponent.m_Type = 666;
		}

		// request create
		{
			auto& requestComponent = world.AddComponent<inventory::StorageCreateRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Limit = 1337;
			requestComponent.m_Type = 666;
		}

		world.Update(2);

		// check result a
		REQUIRE(world.HasComponent<inventory::StorageCreateResultComponent>(requestAEntity));
		const auto& resultAComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Storage != ecs::Entity::Unassigned);
		CHECK(resultAComponent.m_Error == inventory::EError::None);

		// check result b
		REQUIRE(world.HasComponent<inventory::StorageCreateResultComponent>(requestBEntity));
		const auto& resultBComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Storage == ecs::Entity::Unassigned);
		CHECK(resultBComponent.m_Error == inventory::EError::StorageDuplicate);
	}

	SECTION("Failure: Duplicate Storage, Next Frame")
	{
		// request create
		{
			auto& requestComponent = world.AddComponent<inventory::StorageCreateRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Limit = 1337;
			requestComponent.m_Type = 666;
		}

		world.Update();

		// request create
		{
			auto& requestComponent = world.AddComponent<inventory::StorageCreateRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Limit = 1337;
			requestComponent.m_Type = 666;
		}

		world.Update();

		// check result a
		REQUIRE(world.HasComponent<inventory::StorageCreateResultComponent>(requestAEntity));
		const auto& resultAComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Storage != ecs::Entity::Unassigned);
		CHECK(resultAComponent.m_Error == inventory::EError::None);

		world.Update();

		// check result b
		REQUIRE(world.HasComponent<inventory::StorageCreateResultComponent>(requestBEntity));
		const auto& resultBComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Storage == ecs::Entity::Unassigned);
		CHECK(resultBComponent.m_Error == inventory::EError::StorageDuplicate);
	}

	SECTION("Success")
	{
		// request create
		{
			auto& requestComponent = world.AddComponent<inventory::StorageCreateRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Limit = 1337;
			requestComponent.m_Type = 666;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::StorageCreateResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Storage != ecs::Entity::Unassigned);
		CHECK(resultComponent.m_Error == inventory::EError::None);

		// check storage
		REQUIRE(world.HasComponent<inventory::StorageComponent>(resultComponent.m_Storage));
		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(resultComponent.m_Storage);
		CHECK(storageComponent.m_Members.GetCount() == 0);
		CHECK(storageComponent.m_Owner == ownerEntity);
		CHECK(storageComponent.m_Limit == 1337);
		CHECK(storageComponent.m_Type == 666);

		// check changes
		const auto& changesComponent = world.m_EntityWorld.ReadComponent<inventory::StorageChangesComponent>();
		REQUIRE(changesComponent.m_StorageCreated.GetCount() == 1);
		REQUIRE(changesComponent.m_StorageDestroyed.GetCount() == 0);
		CHECK(changesComponent.m_StorageCreated[0].m_Storage == resultComponent.m_Storage);
		CHECK(changesComponent.m_StorageCreated[0].m_Owner == ownerEntity);
		CHECK(changesComponent.m_StorageCreated[0].m_Type == 666);

		world.Update();
		CHECK(changesComponent.m_StorageCreated.GetCount() == 0);
		CHECK(changesComponent.m_StorageDestroyed.GetCount() == 0);
	}
}

TEST_CASE("inventory::StorageSystem::Storage Destroy")
{
	RAIIHelper world;

	const str::Guid transactionId = str::Guid::Generate();
	const ecs::Entity deadEntity = world.CreateEntity();
	const ecs::Entity invalidEntity = world.CreateEntity();
	const ecs::Entity ownerEntity = world.CreateEntity();
	const ecs::Entity requestAEntity = world.CreateEntity();
	const ecs::Entity requestBEntity = world.CreateEntity();
	const ecs::Entity storageEntity = CreateStorage(world, ownerEntity, 666);
	world.Update();
	world.DestroyEntity(deadEntity);
	world.Update();

	SECTION("Failure: Unassigned Storage Entity")
	{
		{
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = ecs::Entity::Unassigned;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Storage == ecs::Entity::Unassigned);
		CHECK(resultComponent.m_Error == inventory::EError::StorageUnassigned);
	}

	SECTION("Failure: Storage Entity is Dead")
	{
		{
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = deadEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Storage == deadEntity);
		CHECK(resultComponent.m_Error == inventory::EError::StorageDead);
	}

	SECTION("Failure: Entity has no Storage")
	{
		{
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = invalidEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Storage == invalidEntity);
		CHECK(resultComponent.m_Error == inventory::EError::StorageMissing);
	}

	SECTION("Failure: Duplicate Request, Same Frame")
	{
		// request destroy
		{
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
		}

		// request destroy
		{
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result a
		REQUIRE(world.HasComponent<inventory::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultAComponent = world.ReadComponent<inventory::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Storage == storageEntity);
		CHECK(resultAComponent.m_Error == inventory::EError::None);

		// check result b
		REQUIRE(world.HasComponent<inventory::StorageDestroyResultComponent>(requestBEntity));
		const auto& resultBComponent = world.ReadComponent<inventory::StorageDestroyResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Storage == storageEntity);
		CHECK(resultBComponent.m_Error == inventory::EError::StorageDead);

		CHECK(!world.IsAlive(storageEntity));
	}

	SECTION("Failure: Duplicate Request, Next Frame")
	{
		// request destroy
		{
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// request destroy
		{
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// check result a
		REQUIRE(world.HasComponent<inventory::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultAComponent = world.ReadComponent<inventory::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Storage == storageEntity);
		CHECK(resultAComponent.m_Error == inventory::EError::None);

		world.Update();

		// check result b
		REQUIRE(world.HasComponent<inventory::StorageDestroyResultComponent>(requestBEntity));
		const auto& resultBComponent = world.ReadComponent<inventory::StorageDestroyResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Storage == storageEntity);
		CHECK(resultBComponent.m_Error == inventory::EError::StorageDead);

		CHECK(!world.IsAlive(storageEntity));
	}

	SECTION("Success")
	{
		{
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<inventory::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultComponent = world.ReadComponent<inventory::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Storage == storageEntity);
		CHECK(resultComponent.m_Error == inventory::EError::None);

		CHECK(!world.IsAlive(storageEntity));

		// check changes
		const auto& changesComponent = world.m_EntityWorld.ReadComponent<inventory::StorageChangesComponent>();
		REQUIRE(changesComponent.m_StorageCreated.GetCount() == 0);
		REQUIRE(changesComponent.m_StorageDestroyed.GetCount() == 1);
		CHECK(changesComponent.m_StorageDestroyed[0].m_Storage == storageEntity);
		CHECK(changesComponent.m_StorageDestroyed[0].m_Owner == ownerEntity);
		CHECK(changesComponent.m_StorageDestroyed[0].m_Type == 666);

		world.Update();
		CHECK(changesComponent.m_StorageCreated.GetCount() == 0);
		CHECK(changesComponent.m_StorageDestroyed.GetCount() == 0);
	}
}

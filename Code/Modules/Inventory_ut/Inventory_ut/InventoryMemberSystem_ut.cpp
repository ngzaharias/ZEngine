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

	ecs::Entity CreateStorage(RAIIHelper& world)
	{
		const ecs::Entity requestEntity = world.CreateEntity();
		world.Update();

		world.AddComponent<inventory::StorageCreateRequestComponent>(requestEntity);
		world.Update(2);

		const auto& resultComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestEntity);
		return resultComponent.m_Storage;
	}
}

TEST_CASE("inventory::MemberSystem::Member Add")
{
	RAIIHelper world;

	const str::Guid transactionId = str::Guid::Generate();
	const ecs::Entity deadEntity = world.CreateEntity();
	const ecs::Entity memberEntity = world.CreateEntity();
	const ecs::Entity requestAEntity = world.CreateEntity();
	const ecs::Entity requestBEntity = world.CreateEntity();
	const ecs::Entity storageEntity = CreateStorage(world);
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(ecs::Entity::Unassigned));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(deadEntity));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(world.HasComponent<inventory::MemberComponent>(memberEntity));

		world.Update();
		CHECK(world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(world.HasComponent<inventory::MemberComponent>(memberEntity));
	}
}

TEST_CASE("inventory::MemberSystem::Member Remove")
{
	RAIIHelper world;

	const str::Guid transactionId = str::Guid::Generate();
	const ecs::Entity deadEntity = world.CreateEntity();
	const ecs::Entity requestAEntity = world.CreateEntity();
	const ecs::Entity requestBEntity = world.CreateEntity();
	const ecs::Entity storageEntity = CreateStorage(world);
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
		CHECK(world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));

		world.Update();
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));
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
		CHECK(!world.HasComponent<inventory::MemberComponent>(memberEntity));
	}
}
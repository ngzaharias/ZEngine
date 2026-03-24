#include <Catch2/catch.hpp>

#include "Core/GameTime.h"
#include "Core/Types.h"
#include "ECS/UTHelpers.h"
#include "ECS/WorldView.h"
#include "ServerInventory/InventoryMemberAddRequestComponent.h"
#include "ServerInventory/InventoryMemberAddResultComponent.h"
#include "ServerInventory/InventoryMemberComponent.h"
#include "ServerInventory/InventoryMemberMoveRequestComponent.h"
#include "ServerInventory/InventoryMemberMoveResultComponent.h"
#include "ServerInventory/InventoryMemberRemoveRequestComponent.h"
#include "ServerInventory/InventoryMemberRemoveResultComponent.h"
#include "ServerInventory/InventoryMemberSystem.h"
#include "ServerInventory/InventoryOwnerComponent.h"
#include "ServerInventory/InventoryOwnerSystem.h"
#include "ServerInventory/InventoryStorageChangesStaticComponent.h"
#include "ServerInventory/InventoryStorageComponent.h"
#include "ServerInventory/InventoryStorageCreateRequestComponent.h"
#include "ServerInventory/InventoryStorageCreateResultComponent.h"
#include "ServerInventory/InventoryStorageDestroyRequestComponent.h"
#include "ServerInventory/InventoryStorageDestroyResultComponent.h"
#include "ServerInventory/InventoryStorageSystem.h"

namespace
{
	struct RAIIHelper : public ut::RAIIHelper
	{
		RAIIHelper()
		{
			m_EntityWorld.RegisterComponent<server::inventory::MemberAddRequestComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::MemberAddResultComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::MemberComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::MemberMoveRequestComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::MemberMoveResultComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::MemberRemoveRequestComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::MemberRemoveResultComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::OwnerComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::StorageComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::StorageCreateRequestComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::StorageCreateResultComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::StorageDestroyRequestComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::StorageDestroyResultComponent>();
			m_EntityWorld.RegisterComponent<server::inventory::StorageChangesStaticComponent>();
			m_EntityWorld.RegisterSystem<server::inventory::StorageSystem>();
			m_EntityWorld.RegisterSystem<server::inventory::MemberSystem>();
			m_EntityWorld.RegisterSystem<server::inventory::OwnerSystem>();

			Initialise();
		}
	};

	ecs::Entity CreateMember(RAIIHelper& world, const ecs::Entity& storageEntity)
	{
		const ecs::Entity memberEntity = world.CreateEntity();
		const ecs::Entity requestEntity = world.CreateEntity();
		world.Update();

		auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestEntity);
		requestComponent.m_Member = memberEntity;
		requestComponent.m_Storage = storageEntity;
		world.Update(2);

		return memberEntity;
	}

	ecs::Entity CreateStorage(RAIIHelper& world)
	{
		const ecs::Entity requestEntity = world.CreateEntity();
		world.Update();

		world.AddComponent<server::inventory::StorageCreateRequestComponent>(requestEntity);
		world.Update(2);

		const auto& resultComponent = world.ReadComponent<server::inventory::StorageCreateResultComponent>(requestEntity);
		return resultComponent.m_Storage;
	}
}

TEST_CASE("server::inventory::MemberSystem::Member Add")
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
			auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = ecs::Entity::Unassigned;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(ecs::Entity::Unassigned));
	}

	SECTION("Member is Dead Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = deadEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(deadEntity));
	}

	SECTION("Storage is Unassigned Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = ecs::Entity::Unassigned;
		}

		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Storage is Dead Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = deadEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Storage Destroyed in Previous Frame")
	{
		// remove storage
		world.RemoveComponent<server::inventory::StorageComponent>(storageEntity);
		world.Update();

		// request add
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Duplicate Request in Previous Frame")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// request add
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();
		CHECK(world.HasComponent<server::inventory::MemberComponent>(memberEntity));

		world.Update();
		CHECK(world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Duplicate Request in Same Frame")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		// request add
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);
		CHECK(world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Regular Flow")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Count = 1337;
			requestComponent.m_GridX = 123;
			requestComponent.m_GridY = 456;
			requestComponent.m_Type = 666;
		}

		world.Update(2);
		CHECK(world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}
}

TEST_CASE("server::inventory::MemberSystem::Member Remove")
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
		REQUIRE(world.HasComponent<server::inventory::MemberComponent>(memberEntity));
		REQUIRE(world.HasComponent<server::inventory::StorageComponent>(storageEntity));
		const auto& memberComponent = world.ReadComponent<server::inventory::MemberComponent>(memberEntity);
		const auto& storageComponent = world.ReadComponent<server::inventory::StorageComponent>(storageEntity);
		REQUIRE(storageComponent.m_Members.Contains(memberEntity));
	}

	SECTION("Member is Unassigned Entity")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = ecs::Entity::Unassigned;
		}

		world.Update(2);
		CHECK(world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Member is Dead Entity")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = deadEntity;
		}

		world.Update(2);
		CHECK(world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Member was Destroyed in Previous Frame")
	{
		// remove member
		world.DestroyEntity(memberEntity);
		world.Update();

		// request remove
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Storage was Destroyed in Previous Frame")
	{
		// destroy the storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<server::inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// request remove
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Storage was Destroyed in Same Frame")
	{
		// destroy the storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<server::inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageEntity;
		}

		// request remove
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberRemoveRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Duplicate Request in Previous Frame")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update();

		// request remove
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberRemoveRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update();
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));

		world.Update();
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Duplicate Request in Same Frame")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		// request remove
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberRemoveRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Member and Storage are Destroyed in Same Frame")
	{
		// destroy the storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<server::inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageEntity;
		}

		world.DestroyEntity(memberEntity);
		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}

	SECTION("Regular Flow")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<server::inventory::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<server::inventory::MemberComponent>(memberEntity));
	}
}
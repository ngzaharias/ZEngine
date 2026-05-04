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

	ecs::Entity CreateStorage(RAIIHelper& world, const ecs::Entity& ownerEntity, const int32 type)
	{
		const ecs::Entity requestEntity = world.CreateEntity();
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

TEST_CASE("inventory::OwnerSystem::Owner Added")
{
	RAIIHelper world;

	const ecs::Entity deadEntity = world.CreateEntity();
	const ecs::Entity ownerEntity = world.CreateEntity();
	world.Update();
	world.DestroyEntity(deadEntity);
	world.Update();

	SECTION("Unassigned Owner Entity")
	{
		const ecs::Entity storageEntity = CreateStorage(world, ecs::Entity::Unassigned, 0);
		world.Update();

		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(storageEntity);
		CHECK(storageComponent.m_Owner == ecs::Entity::Unassigned);
	}

	SECTION("Owner Entity is Dead")
	{
		const ecs::Entity storageEntity = CreateStorage(world, deadEntity, 0);
		world.Update();

		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(storageEntity);
		CHECK(storageComponent.m_Owner == deadEntity);
	}

	SECTION("Owner Entity is Valid")
	{
		const ecs::Entity storageEntity = CreateStorage(world, ownerEntity, 0);
		world.Update();

		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(storageEntity);
		CHECK(storageComponent.m_Owner == ownerEntity);

		REQUIRE(world.HasComponent<inventory::OwnerComponent>(ownerEntity));
		const auto& ownerComponent = world.ReadComponent<inventory::OwnerComponent>(ownerEntity);
		CHECK(ownerComponent.m_Storages.GetCount() == 1);
		CHECK(ownerComponent.m_Storages.Contains(0));
		CHECK(ownerComponent.m_Storages.Get(0) == storageEntity);
	}

	SECTION("Owner Entity has multiple Storages, Same Frame")
	{
		const ecs::Entity requestAEntity = world.CreateEntity();
		const ecs::Entity requestBEntity = world.CreateEntity();
		auto& requestAComponent = world.AddComponent<inventory::StorageCreateRequestComponent>(requestAEntity);
		requestAComponent.m_Owner = ownerEntity;
		requestAComponent.m_Type = 0;
		auto& requestBComponent = world.AddComponent<inventory::StorageCreateRequestComponent>(requestBEntity);
		requestBComponent.m_Owner = ownerEntity;
		requestBComponent.m_Type = 1;
		world.Update(2);

		const auto& resultAComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestAEntity);
		const auto& resultBComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestBEntity);
		const auto& storageAComponent = world.ReadComponent<inventory::StorageComponent>(resultAComponent.m_Storage);
		const auto& storageBComponent = world.ReadComponent<inventory::StorageComponent>(resultBComponent.m_Storage);
		CHECK(storageAComponent.m_Owner == ownerEntity);
		CHECK(storageBComponent.m_Owner == ownerEntity);
	}

	SECTION("Owner Entity has multiple Storages, Next Frame")
	{
		const ecs::Entity storageAEntity = CreateStorage(world, ownerEntity, 0);
		const ecs::Entity storageBEntity = CreateStorage(world, ownerEntity, 1);

		const auto& storageAComponent = world.ReadComponent<inventory::StorageComponent>(storageAEntity);
		CHECK(storageAComponent.m_Owner == ownerEntity);

		const auto& storageBComponent = world.ReadComponent<inventory::StorageComponent>(storageBEntity);
		CHECK(storageBComponent.m_Owner == ownerEntity);
	}
}

TEST_CASE("inventory::OwnerSystem::Owner Removed")
{
	RAIIHelper world;

	const ecs::Entity ownerEntity = world.CreateEntity();
	world.Update();

	SECTION("Remove 1 of 1 Storage")
	{
		const ecs::Entity storageEntity = CreateStorage(world, ownerEntity, 0);
		REQUIRE(world.HasComponent<inventory::OwnerComponent>(ownerEntity));

		DestroyStorage(world, storageEntity);
		CHECK(!world.HasComponent<inventory::OwnerComponent>(ownerEntity));

		world.Update();
	}

	SECTION("Remove 1 of 2 Storage")
	{
		const ecs::Entity storageAEntity = CreateStorage(world, ownerEntity, 0);
		const ecs::Entity storageBEntity = CreateStorage(world, ownerEntity, 1);
		REQUIRE(world.HasComponent<inventory::OwnerComponent>(ownerEntity));

		DestroyStorage(world, storageAEntity);

		REQUIRE(world.HasComponent<inventory::OwnerComponent>(ownerEntity));
		const auto& ownerComponent = world.ReadComponent<inventory::OwnerComponent>(ownerEntity);
		CHECK(ownerComponent.m_Storages.GetCount() == 1);
		CHECK(ownerComponent.m_Storages.Contains(1));
		CHECK(ownerComponent.m_Storages.Get(1) == storageBEntity);
	}

	SECTION("Remove 2 of 2 Storage")
	{
		const ecs::Entity storageAEntity = CreateStorage(world, ownerEntity, 0);
		const ecs::Entity storageBEntity = CreateStorage(world, ownerEntity, 1);
		DestroyStorage(world, storageAEntity);
		DestroyStorage(world, storageBEntity);

		CHECK(!world.HasComponent<inventory::OwnerComponent>(ownerEntity));
	}

	SECTION("Destroy then Create, Same Frame")
	{
		const ecs::Entity requestAEntity = world.CreateEntity();
		const ecs::Entity requestBEntity = world.CreateEntity();
		const ecs::Entity storageAEntity = CreateStorage(world, ownerEntity, 0);

		// destroy the previous storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageAEntity;
		}

		// make a new storage
		{
			auto& requestComponent = world.AddComponent<inventory::StorageCreateRequestComponent>(requestBEntity);
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Type = 1;
		}

		world.Update(2);

		REQUIRE(world.HasComponent<inventory::StorageCreateResultComponent>(requestBEntity));
		const auto& resultComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestBEntity);
		REQUIRE(resultComponent.m_Error == inventory::EError::None);

		const ecs::Entity storageBEntity = resultComponent.m_Storage;
		REQUIRE(world.HasComponent<inventory::OwnerComponent>(ownerEntity));
		const auto& ownerComponent = world.ReadComponent<inventory::OwnerComponent>(ownerEntity);
		CHECK(ownerComponent.m_Storages.GetCount() == 1);
		CHECK(ownerComponent.m_Storages.Contains(1));
		CHECK(ownerComponent.m_Storages.Get(1) == storageBEntity);
	}

	SECTION("Destroy then Create, Next Frame")
	{
		const ecs::Entity requestAEntity = world.CreateEntity();
		const ecs::Entity requestBEntity = world.CreateEntity();
		const ecs::Entity storageAEntity = CreateStorage(world, ownerEntity, 0);

		// destroy the previous storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<inventory::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageAEntity;
		}

		world.Update();

		// make a new storage
		{
			auto& requestComponent = world.AddComponent<inventory::StorageCreateRequestComponent>(requestBEntity);
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Type = 1;
		}

		world.Update(2);

		REQUIRE(world.HasComponent<inventory::StorageCreateResultComponent>(requestBEntity));
		const auto& resultComponent = world.ReadComponent<inventory::StorageCreateResultComponent>(requestBEntity);
		REQUIRE(resultComponent.m_Error == inventory::EError::None);

		const ecs::Entity storageBEntity = resultComponent.m_Storage;
		REQUIRE(world.HasComponent<inventory::OwnerComponent>(ownerEntity));
		const auto& ownerComponent = world.ReadComponent<inventory::OwnerComponent>(ownerEntity);
		CHECK(ownerComponent.m_Storages.GetCount() == 1);
		CHECK(ownerComponent.m_Storages.Contains(1));
		CHECK(ownerComponent.m_Storages.Get(1) == storageBEntity);
	}
}
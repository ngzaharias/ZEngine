#include <Catch2/catch.hpp>

#include <Core/GameTime.h>
#include <Core/Types.h>

#include <ECS/UTHelpers.h>
#include <ECS/WorldView.h>

#include <GameClient/ContainerComponents.h>
#include <GameClient/ContainerMemberSystem.h>
#include <GameClient/ContainerOwnerSystem.h>
#include <GameClient/ContainerStorageSystem.h>

namespace
{
	struct RAIIHelper : public ut::RAIIHelper
	{
		RAIIHelper()
		{
			m_EntityWorld.RegisterComponent<container::MemberAddRequestComponent>();
			m_EntityWorld.RegisterComponent<container::MemberAddResultComponent>();
			m_EntityWorld.RegisterComponent<container::MemberChangesComponent>();
			m_EntityWorld.RegisterComponent<container::MemberComponent>();
			m_EntityWorld.RegisterComponent<container::MemberRemoveRequestComponent>();
			m_EntityWorld.RegisterComponent<container::MemberRemoveResultComponent>();
			m_EntityWorld.RegisterComponent<container::OwnerComponent>();
			m_EntityWorld.RegisterComponent<container::StorageChangesComponent>();
			m_EntityWorld.RegisterComponent<container::StorageComponent>();
			m_EntityWorld.RegisterComponent<container::StorageCreateRequestComponent>();
			m_EntityWorld.RegisterComponent<container::StorageCreateResultComponent>();
			m_EntityWorld.RegisterComponent<container::StorageDestroyRequestComponent>();
			m_EntityWorld.RegisterComponent<container::StorageDestroyResultComponent>();
			m_EntityWorld.RegisterSystem<container::MemberSystem>();
			m_EntityWorld.RegisterSystem<container::StorageSystem>();
			m_EntityWorld.RegisterSystem<container::OwnerSystem>();

			Initialise();
		}
	};

	ecs::Entity CreateStorage(RAIIHelper& world, const ecs::Entity& ownerEntity, const int32 type)
	{
		const ecs::Entity requestEntity = world.CreateEntity();
		world.Update();

		auto& requestComponent = world.AddComponent<container::StorageCreateRequestComponent>(requestEntity);
		requestComponent.m_Owner = ownerEntity;
		requestComponent.m_Type = type;
		world.Update(2);

		if (!world.HasComponent<container::StorageCreateResultComponent>(requestEntity))
			return ecs::Entity::Unassigned;

		const auto& resultComponent = world.GetComponent<const container::StorageCreateResultComponent>(requestEntity);
		return resultComponent.m_Storage;
	}
}

CATCH_REGISTER_ENUM(container::EError, 
	container::EError::None,
	container::EError::MemberDead,
	container::EError::MemberDuplicate,
	container::EError::MemberMissing,
	container::EError::MemberUnassigned,
	container::EError::OwnerDead,
	container::EError::OwnerDestroyed,
	container::EError::OwnerInvalid,
	container::EError::OwnerUnassigned,
	container::EError::StorageDead,
	container::EError::StorageDuplicate,
	container::EError::StorageMissing,
	container::EError::StorageUnassigned);

TEST_CASE("container::StorageSystem::Member Add")
{
	RAIIHelper world;

	const ecs::Entity storageEntity = CreateStorage(world, ecs::Entity::Unassigned, 0);
	world.Update();
}

TEST_CASE("container::StorageSystem::Member Remove")
{
	RAIIHelper world;

	const ecs::Entity storageEntity = CreateStorage(world, ecs::Entity::Unassigned, 0);
	world.Update();
}

TEST_CASE("container::StorageSystem::Storage Create")
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
			auto& requestComponent = world.AddComponent<container::StorageCreateRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Limit = 1337;
			requestComponent.m_Type = 666;
		}

		// request create
		{
			auto& requestComponent = world.AddComponent<container::StorageCreateRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Limit = 1337;
			requestComponent.m_Type = 666;
		}

		world.Update(2);

		// check result a
		REQUIRE(world.HasComponent<container::StorageCreateResultComponent>(requestAEntity));
		const auto& resultAComponent = world.GetComponent<const container::StorageCreateResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Storage != ecs::Entity::Unassigned);
		CHECK(resultAComponent.m_Error == container::EError::None);

		// check result b
		REQUIRE(world.HasComponent<container::StorageCreateResultComponent>(requestBEntity));
		const auto& resultBComponent = world.GetComponent<const container::StorageCreateResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Storage == ecs::Entity::Unassigned);
		CHECK(resultBComponent.m_Error == container::EError::StorageDuplicate);
	}

	SECTION("Failure: Duplicate Storage, Next Frame")
	{
		// request create
		{
			auto& requestComponent = world.AddComponent<container::StorageCreateRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Limit = 1337;
			requestComponent.m_Type = 666;
		}

		world.Update();

		// request create
		{
			auto& requestComponent = world.AddComponent<container::StorageCreateRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Limit = 1337;
			requestComponent.m_Type = 666;
		}

		world.Update();

		// check result a
		REQUIRE(world.HasComponent<container::StorageCreateResultComponent>(requestAEntity));
		const auto& resultAComponent = world.GetComponent<const container::StorageCreateResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Storage != ecs::Entity::Unassigned);
		CHECK(resultAComponent.m_Error == container::EError::None);

		world.Update();

		// check result b
		REQUIRE(world.HasComponent<container::StorageCreateResultComponent>(requestBEntity));
		const auto& resultBComponent = world.GetComponent<const container::StorageCreateResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Storage == ecs::Entity::Unassigned);
		CHECK(resultBComponent.m_Error == container::EError::StorageDuplicate);
	}

	SECTION("Success")
	{
		// request create
		{
			auto& requestComponent = world.AddComponent<container::StorageCreateRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Owner = ownerEntity;
			requestComponent.m_Limit = 1337;
			requestComponent.m_Type = 666;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::StorageCreateResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::StorageCreateResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Storage != ecs::Entity::Unassigned);
		CHECK(resultComponent.m_Error == container::EError::None);

		// check storage
		REQUIRE(world.HasComponent<container::StorageComponent>(resultComponent.m_Storage));
		const auto& storageComponent = world.GetComponent<const container::StorageComponent>(resultComponent.m_Storage);
		CHECK(storageComponent.m_Members.GetCount() == 0);
		CHECK(storageComponent.m_Owner == ownerEntity);
		CHECK(storageComponent.m_Limit == 1337);
		CHECK(storageComponent.m_Type == 666);

		// check changes
		const auto& changesComponent = world.m_EntityWorld.GetSingleton<const container::StorageChangesComponent>();
		REQUIRE(changesComponent.m_Created.GetCount() == 1);
		REQUIRE(changesComponent.m_Destroyed.GetCount() == 0);
		CHECK(changesComponent.m_Created[0].m_Storage == resultComponent.m_Storage);
		CHECK(changesComponent.m_Created[0].m_Owner == ownerEntity);
		CHECK(changesComponent.m_Created[0].m_Type == 666);

		world.Update();
		CHECK(changesComponent.m_Created.GetCount() == 0);
		CHECK(changesComponent.m_Destroyed.GetCount() == 0);
	}
}

TEST_CASE("container::StorageSystem::Storage Destroy")
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
	world.DestoryEntity(deadEntity);
	world.Update();

	SECTION("Failure: Unassigned Storage Entity")
	{
		{
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = ecs::Entity::Unassigned;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Storage == ecs::Entity::Unassigned);
		CHECK(resultComponent.m_Error == container::EError::StorageUnassigned);
	}

	SECTION("Failure: Storage Entity is Dead")
	{
		{
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = deadEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Storage == deadEntity);
		CHECK(resultComponent.m_Error == container::EError::StorageDead);
	}

	SECTION("Failure: Entity has no Storage")
	{
		{
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = invalidEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Storage == invalidEntity);
		CHECK(resultComponent.m_Error == container::EError::StorageMissing);
	}

	SECTION("Failure: Duplicate Request, Same Frame")
	{
		// request destroy
		{
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
		}

		// request destroy
		{
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result a
		REQUIRE(world.HasComponent<container::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultAComponent = world.GetComponent<const container::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Storage == storageEntity);
		CHECK(resultAComponent.m_Error == container::EError::None);

		// check result b
		REQUIRE(world.HasComponent<container::StorageDestroyResultComponent>(requestBEntity));
		const auto& resultBComponent = world.GetComponent<const container::StorageDestroyResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Storage == storageEntity);
		CHECK(resultBComponent.m_Error == container::EError::StorageDead);

		CHECK(!world.IsAlive(storageEntity));
	}

	SECTION("Failure: Duplicate Request, Next Frame")
	{
		// request destroy
		{
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// request destroy
		{
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// check result a
		REQUIRE(world.HasComponent<container::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultAComponent = world.GetComponent<const container::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Storage == storageEntity);
		CHECK(resultAComponent.m_Error == container::EError::None);

		world.Update();

		// check result b
		REQUIRE(world.HasComponent<container::StorageDestroyResultComponent>(requestBEntity));
		const auto& resultBComponent = world.GetComponent<const container::StorageDestroyResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Storage == storageEntity);
		CHECK(resultBComponent.m_Error == container::EError::StorageDead);

		CHECK(!world.IsAlive(storageEntity));
	}

	SECTION("Success")
	{
		{
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::StorageDestroyResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::StorageDestroyResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Storage == storageEntity);
		CHECK(resultComponent.m_Error == container::EError::None);

		CHECK(!world.IsAlive(storageEntity));

		// check changes
		const auto& changesComponent = world.m_EntityWorld.GetSingleton<const container::StorageChangesComponent>();
		REQUIRE(changesComponent.m_Created.GetCount() == 0);
		REQUIRE(changesComponent.m_Destroyed.GetCount() == 1);
		CHECK(changesComponent.m_Destroyed[0].m_Storage == storageEntity);
		CHECK(changesComponent.m_Destroyed[0].m_Owner == ownerEntity);
		CHECK(changesComponent.m_Destroyed[0].m_Type == 666);

		world.Update();
		CHECK(changesComponent.m_Created.GetCount() == 0);
		CHECK(changesComponent.m_Destroyed.GetCount() == 0);
	}
}

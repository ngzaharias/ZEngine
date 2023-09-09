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

	ecs::Entity CreateMember(RAIIHelper& world, const ecs::Entity& storageEntity)
	{
		const ecs::Entity memberEntity = world.CreateEntity();
		const ecs::Entity requestEntity = world.CreateEntity();
		world.Update();

		auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestEntity);
		requestComponent.m_Member = memberEntity;
		requestComponent.m_Storage = storageEntity;
		world.Update(2);

		return memberEntity;
	}

	ecs::Entity CreateStorage(RAIIHelper& world)
	{
		const ecs::Entity requestEntity = world.CreateEntity();
		world.Update();

		world.AddComponent<container::StorageCreateRequestComponent>(requestEntity);
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

TEST_CASE("container::MemberSystem::Member Add")
{
	RAIIHelper world;

	const str::Guid transactionId = str::Guid::Generate();
	const ecs::Entity deadEntity = world.CreateEntity();
	const ecs::Entity memberEntity = world.CreateEntity();
	const ecs::Entity requestAEntity = world.CreateEntity();
	const ecs::Entity requestBEntity = world.CreateEntity();
	const ecs::Entity storageEntity = CreateStorage(world);
	world.Update();
	world.DestoryEntity(deadEntity);
	world.Update();

	SECTION("Failure: Unassigned Member Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = ecs::Entity::Unassigned;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == ecs::Entity::Unassigned);
		CHECK(resultComponent.m_Error == container::EError::MemberUnassigned);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(resultComponent.m_Member));
	}

	SECTION("Failure: Dead Member Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = deadEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == deadEntity);
		CHECK(resultComponent.m_Error == container::EError::MemberDead);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(resultComponent.m_Member));
	}

	SECTION("Failure: Duplicate Member")
	{
		// make a member
		world.AddComponent<container::MemberComponent>(memberEntity);
		world.Update();

		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == container::EError::MemberDuplicate);
	}

	SECTION("Failure: Unassigned Storage Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = ecs::Entity::Unassigned;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == container::EError::StorageUnassigned);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(resultComponent.m_Member));
	}

	SECTION("Failure: Dead Storage Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = deadEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == container::EError::StorageDead);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(resultComponent.m_Member));
	}

	SECTION("Failure: Missing Storage")
	{
		// remove storage
		world.RemoveComponent<container::StorageComponent>(storageEntity);
		world.Update();

		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == container::EError::StorageMissing);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(resultComponent.m_Member));
	}

	SECTION("Failure: Duplicate Member, Same Frame")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);

		// check result a
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestAEntity));
		const auto& resultAComponent = world.GetComponent<const container::MemberAddResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Member == memberEntity);
		CHECK(resultAComponent.m_Error == container::EError::None);

		// check result b
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestBEntity));
		const auto& resultBComponent = world.GetComponent<const container::MemberAddResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Member == memberEntity);
		CHECK(resultBComponent.m_Error == container::EError::MemberDuplicate);
	}

	SECTION("Failure: Duplicate Member, Next Frame")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// check result a
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestAEntity));
		const auto& resultAComponent = world.GetComponent<const container::MemberAddResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Member == memberEntity);
		CHECK(resultAComponent.m_Error == container::EError::None);

		world.Update();

		// check result b
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestBEntity));
		const auto& resultBComponent = world.GetComponent<const container::MemberAddResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Member == memberEntity);
		CHECK(resultBComponent.m_Error == container::EError::MemberDuplicate);
	}

	SECTION("Success")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
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
		REQUIRE(world.HasComponent<container::MemberAddResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberAddResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == container::EError::None);

		// check member
		REQUIRE(world.HasComponent<container::MemberComponent>(resultComponent.m_Member));
		const auto& memberComponent = world.GetComponent<const container::MemberComponent>(resultComponent.m_Member);
		CHECK(memberComponent.m_Storage == storageEntity);
		CHECK(memberComponent.m_Count == 1337);
		CHECK(memberComponent.m_GridX == 123);
		CHECK(memberComponent.m_GridY == 456);
		CHECK(memberComponent.m_Type == 666);

		// checks changes
		const auto& changesComponent = world.m_EntityWorld.GetSingleton<const container::MemberChangesComponent>();
		REQUIRE(changesComponent.m_Added.GetCount() == 1);
		REQUIRE(changesComponent.m_Removed.GetCount() == 0);
		CHECK(changesComponent.m_Added[0].m_Member == memberEntity);
		CHECK(changesComponent.m_Added[0].m_Storage == storageEntity);

		world.Update();
		CHECK(changesComponent.m_Added.GetCount() == 0);
		CHECK(changesComponent.m_Removed.GetCount() == 0);
	}
}

TEST_CASE("container::MemberSystem::Member Remove")
{
	RAIIHelper world;

	const str::Guid transactionId = str::Guid::Generate();
	const ecs::Entity deadEntity = world.CreateEntity();
	const ecs::Entity requestAEntity = world.CreateEntity();
	const ecs::Entity requestBEntity = world.CreateEntity();
	const ecs::Entity storageEntity = CreateStorage(world);
	const ecs::Entity memberEntity = CreateMember(world, storageEntity);
	world.Update();
	world.DestoryEntity(deadEntity);
	world.Update();

	{
		REQUIRE(world.HasComponent<container::MemberComponent>(memberEntity));
		REQUIRE(world.HasComponent<container::StorageComponent>(storageEntity));
		const auto& memberComponent = world.GetComponent<const container::MemberComponent>(memberEntity);
		const auto& storageComponent = world.GetComponent<const container::StorageComponent>(storageEntity);
		REQUIRE(storageComponent.m_Members.Contains(memberEntity));
	}

	SECTION("Failure: Unassigned Member Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = ecs::Entity::Unassigned;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == ecs::Entity::Unassigned);
		CHECK(resultComponent.m_Error == container::EError::MemberUnassigned);

		// check member
		CHECK(world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Failure: Dead Member Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = deadEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == deadEntity);
		CHECK(resultComponent.m_Error == container::EError::MemberDead);

		// check member
		CHECK(world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Failure: Missing Member")
	{
		// remove member
		world.RemoveComponent<container::MemberComponent>(memberEntity);
		world.Update();

		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == container::EError::MemberMissing);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Failure: Dead Storage Entity, Same Frame")
	{
		// destroy the storage
		world.DestoryEntity(storageEntity);

		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == container::EError::StorageDead);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Failure: Dead Storage Entity, Next Frame")
	{
		// destroy the storage
		world.DestoryEntity(storageEntity);
		world.Update();

		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == container::EError::MemberMissing);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Failure: Duplicate Request, Same Frame")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);

		// check result a
		REQUIRE(world.HasComponent<container::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultAComponent = world.GetComponent<const container::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Member == memberEntity);
		CHECK(resultAComponent.m_Error == container::EError::None);

		// check result b
		REQUIRE(world.HasComponent<container::MemberRemoveResultComponent>(requestBEntity));
		const auto& resultBComponent = world.GetComponent<const container::MemberRemoveResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Member == memberEntity);
		CHECK(resultBComponent.m_Error == container::EError::MemberDead);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Failure: Duplicate Request, Next Frame")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update();

		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update();

		// check result a
		REQUIRE(world.HasComponent<container::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultAComponent = world.GetComponent<const container::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultAComponent.m_TransactionId == transactionId);
		CHECK(resultAComponent.m_Member == memberEntity);
		CHECK(resultAComponent.m_Error == container::EError::None);

		world.Update();

		// check result b
		REQUIRE(world.HasComponent<container::MemberRemoveResultComponent>(requestBEntity));
		const auto& resultBComponent = world.GetComponent<const container::MemberRemoveResultComponent>(requestBEntity);
		CHECK(resultBComponent.m_TransactionId == transactionId);
		CHECK(resultBComponent.m_Member == memberEntity);
		CHECK(resultBComponent.m_Error == container::EError::MemberMissing);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Success")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);

		// check result
		REQUIRE(world.HasComponent<container::MemberRemoveResultComponent>(requestAEntity));
		const auto& resultComponent = world.GetComponent<const container::MemberRemoveResultComponent>(requestAEntity);
		CHECK(resultComponent.m_TransactionId == transactionId);
		CHECK(resultComponent.m_Member == memberEntity);
		CHECK(resultComponent.m_Error == container::EError::None);

		// check member
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));

		// check changes
		const auto& changesComponent = world.m_EntityWorld.GetSingleton<const container::MemberChangesComponent>();
		REQUIRE(changesComponent.m_Added.GetCount() == 0);
		REQUIRE(changesComponent.m_Removed.GetCount() == 1);
		CHECK(changesComponent.m_Removed[0].m_Member == memberEntity);
		CHECK(changesComponent.m_Removed[0].m_Storage == storageEntity);

		world.Update();
		CHECK(changesComponent.m_Added.GetCount() == 0);
		CHECK(changesComponent.m_Removed.GetCount() == 0);
	}
}
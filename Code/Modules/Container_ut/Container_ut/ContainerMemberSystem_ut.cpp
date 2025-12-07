#include <Catch2/catch.hpp>

#include "Container/ContainerComponents.h"
#include "Container/ContainerMemberSystem.h"
#include "Container/ContainerOwnerSystem.h"
#include "Container/ContainerStorageSystem.h"
#include "Core/GameTime.h"
#include "Core/Types.h"
#include "ECS/UTHelpers.h"
#include "ECS/WorldView.h"

namespace
{
	struct RAIIHelper : public ut::RAIIHelper
	{
		RAIIHelper()
		{
			m_EntityWorld.RegisterComponent<container::MemberAddRequestComponent>();
			m_EntityWorld.RegisterComponent<container::MemberAddResultComponent>();
			m_EntityWorld.RegisterComponent<container::MemberComponent>();
			m_EntityWorld.RegisterComponent<container::MemberMoveRequestComponent>();
			m_EntityWorld.RegisterComponent<container::MemberMoveResultComponent>();
			m_EntityWorld.RegisterComponent<container::MemberRemoveRequestComponent>();
			m_EntityWorld.RegisterComponent<container::MemberRemoveResultComponent>();
			m_EntityWorld.RegisterComponent<container::OwnerComponent>();
			m_EntityWorld.RegisterComponent<container::StorageComponent>();
			m_EntityWorld.RegisterComponent<container::StorageCreateRequestComponent>();
			m_EntityWorld.RegisterComponent<container::StorageCreateResultComponent>();
			m_EntityWorld.RegisterComponent<container::StorageDestroyRequestComponent>();
			m_EntityWorld.RegisterComponent<container::StorageDestroyResultComponent>();
			m_EntityWorld.RegisterSingleton<container::StorageChangesSingleton>();
			m_EntityWorld.RegisterSystem<container::StorageSystem>();
			m_EntityWorld.RegisterSystem<container::MemberSystem>();
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

		const auto& resultComponent = world.ReadComponent<container::StorageCreateResultComponent>(requestEntity);
		return resultComponent.m_Storage;
	}
}

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
	world.DestroyEntity(deadEntity);
	world.Update();

	SECTION("Member is Unassigned Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = ecs::Entity::Unassigned;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<container::MemberComponent>(ecs::Entity::Unassigned));
	}

	SECTION("Member is Dead Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = deadEntity;
			requestComponent.m_Storage = storageEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<container::MemberComponent>(deadEntity));
	}

	SECTION("Storage is Unassigned Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = ecs::Entity::Unassigned;
		}

		world.Update(2);
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Storage is Dead Entity")
	{
		// request add
		{
			auto& requestComponent = world.AddComponent<container::MemberAddRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
			requestComponent.m_Storage = deadEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Storage Destroyed in Previous Frame")
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
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Duplicate Request in Previous Frame")
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
		CHECK(world.HasComponent<container::MemberComponent>(memberEntity));

		world.Update();
		CHECK(world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Duplicate Request in Same Frame")
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
		CHECK(world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Regular Flow")
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
		CHECK(world.HasComponent<container::MemberComponent>(memberEntity));
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
	world.DestroyEntity(deadEntity);
	world.Update();

	{
		REQUIRE(world.HasComponent<container::MemberComponent>(memberEntity));
		REQUIRE(world.HasComponent<container::StorageComponent>(storageEntity));
		const auto& memberComponent = world.ReadComponent<container::MemberComponent>(memberEntity);
		const auto& storageComponent = world.ReadComponent<container::StorageComponent>(storageEntity);
		REQUIRE(storageComponent.m_Members.Contains(memberEntity));
	}

	SECTION("Member is Unassigned Entity")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = ecs::Entity::Unassigned;
		}

		world.Update(2);
		CHECK(world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Member is Dead Entity")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = deadEntity;
		}

		world.Update(2);
		CHECK(world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Member was Destroyed in Previous Frame")
	{
		// remove member
		world.DestroyEntity(memberEntity);
		world.Update();

		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Storage was Destroyed in Previous Frame")
	{
		// destroy the storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageEntity;
		}

		world.Update();

		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Storage was Destroyed in Same Frame")
	{
		// destroy the storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageEntity;
		}

		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestBEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Duplicate Request in Previous Frame")
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
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));

		world.Update();
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Duplicate Request in Same Frame")
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
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Member and Storage are Destroyed in Same Frame")
	{
		// destroy the storage
		{
			const ecs::Entity requestEntity = world.CreateEntity();
			auto& requestComponent = world.AddComponent<container::StorageDestroyRequestComponent>(requestAEntity);
			requestComponent.m_Storage = storageEntity;
		}

		world.DestroyEntity(memberEntity);
		world.Update(2);
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}

	SECTION("Regular Flow")
	{
		// request remove
		{
			auto& requestComponent = world.AddComponent<container::MemberRemoveRequestComponent>(requestAEntity);
			requestComponent.m_TransactionId = transactionId;
			requestComponent.m_Member = memberEntity;
		}

		world.Update(2);
		CHECK(!world.HasComponent<container::MemberComponent>(memberEntity));
	}
}
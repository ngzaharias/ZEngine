#include "GameClientPCH.h"
#include "GameClient/ContainerMemberSystem.h"

#include <Core/Algorithms.h>
#include <Core/Nullable.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "GameClient/ContainerComponents.h"

namespace
{
	using World = container::MemberSystem::World;

	container::EError VerifyMemberAdd(World& world, const ecs::Entity& entity, const container::MemberChangesComponent& frameData)
	{
		const auto& requestComponent = world.GetComponent<const container::MemberAddRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return container::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return container::EError::MemberDead;
		if (world.HasComponent<container::MemberComponent>(requestComponent.m_Member))
			return container::EError::MemberDuplicate;

		if (requestComponent.m_Storage.IsUnassigned())
			return container::EError::StorageUnassigned;
		if (!world.IsAlive(requestComponent.m_Storage))
			return container::EError::StorageDead;
		if (!world.HasComponent<container::StorageComponent>(requestComponent.m_Storage))
			return container::EError::StorageMissing;

		auto hasDuplicate = [&](const container::MemberChange& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (core::ContainsIf(frameData.m_Added, hasDuplicate))
			return container::EError::MemberDuplicate;

		return container::EError::None;
	}

	container::EError VerifyMemberRemove(World& world, const ecs::Entity& entity, const container::MemberChangesComponent& frameData)
	{
		const auto& requestComponent = world.GetComponent<const container::MemberRemoveRequestComponent>(entity);
		if (requestComponent.m_Member.IsUnassigned())
			return container::EError::MemberUnassigned;
		if (!world.IsAlive(requestComponent.m_Member))
			return container::EError::MemberDead;
		if (!world.HasComponent<container::MemberComponent>(requestComponent.m_Member))
			return container::EError::MemberMissing;

		const auto& memberComponent = world.GetComponent<const container::MemberComponent>(requestComponent.m_Member);
		if (!world.IsAlive(memberComponent.m_Storage))
			return container::EError::StorageDead;

		auto hasDuplicate = [&](const container::MemberChange& rhs)
		{
			return requestComponent.m_Member == rhs.m_Member;
		};

		if (core::ContainsIf(frameData.m_Removed, hasDuplicate))
			return container::EError::MemberDead;

		return container::EError::None;
	}
}

void container::MemberSystem::Initialise(World& world)
{
	world.AddSingleton<container::MemberChangesComponent>();
}

void container::MemberSystem::Shutdown(World& world)
{
	world.RemoveSingleton<container::MemberChangesComponent>();
}

void container::MemberSystem::Update(World& world, const GameTime& gameTime)
{
	ProcessRequests(world);
}

void container::MemberSystem::ProcessRequests(World& world)
{
	auto& changesComponent = world.GetSingleton<container::MemberChangesComponent>();
	changesComponent.m_Added.RemoveAll();
	changesComponent.m_Removed.RemoveAll();

	for (const ecs::Entity& requestEntity : world.Query<ecs::query::Added<const container::MemberAddRequestComponent>>())
	{
		const EError error = VerifyMemberAdd(world, requestEntity, changesComponent);

		const auto& requestComponent = world.GetComponent<const container::MemberAddRequestComponent>(requestEntity);
		auto& resultComponent = world.AddComponent<container::MemberAddResultComponent>(requestEntity);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			auto& storageComponent = world.GetComponent<container::StorageComponent>(requestComponent.m_Storage);
			storageComponent.m_Members.Add(requestComponent.m_Member);

			auto& memberComponent = world.AddComponent<container::MemberComponent>(requestComponent.m_Member);
			memberComponent.m_Storage = requestComponent.m_Storage;
			memberComponent.m_Count = requestComponent.m_Count;
			memberComponent.m_GridX = requestComponent.m_GridX;
			memberComponent.m_GridY = requestComponent.m_GridY;
			memberComponent.m_Type = requestComponent.m_Type;

			// add to frame data so StorageSystem can add it to the storage
			MemberChange& createData = changesComponent.m_Added.Emplace();
			createData.m_Member = requestComponent.m_Member;
			createData.m_Storage = requestComponent.m_Storage;
		}
	}

	// gather remove requests to handle duplicates
	Set<ecs::Entity> removeRequests;
	for (const ecs::Entity& requestEntity : world.Query<ecs::query::Added<const container::MemberRemoveRequestComponent>>())
	{
		const EError error = VerifyMemberRemove(world, requestEntity, changesComponent);

		const auto& requestComponent = world.GetComponent<const container::MemberRemoveRequestComponent>(requestEntity);
		auto& resultComponent = world.AddComponent<container::MemberRemoveResultComponent>(requestEntity);
		resultComponent.m_TransactionId = requestComponent.m_TransactionId;
		resultComponent.m_Member = requestComponent.m_Member;
		resultComponent.m_Error = error;

		if (error == EError::None)
		{
			const auto& memberComponent = world.GetComponent<const container::MemberComponent>(requestComponent.m_Member);
			removeRequests.Add(requestComponent.m_Member);

			// add to frame data so StorageSystem can remove it from the storage
			MemberChange& destroyData = changesComponent.m_Removed.Emplace();
			destroyData.m_Member = requestComponent.m_Member;
			destroyData.m_Storage = memberComponent.m_Storage;
		}
	}

	for (const ecs::Entity& storageEntity : world.Query<ecs::query::Removed<const container::StorageComponent>>())
	{
		const auto& storageComponent = world.GetComponent<const container::StorageComponent>(storageEntity, false);
		for (const ecs::Entity& memberEntity : storageComponent.m_Members)
			removeRequests.Add(memberEntity);
	}

	// process requests, no safety checks as that indicates an error in the system
	for (const ecs::Entity& memberEntity : removeRequests)
		world.RemoveComponent<container::MemberComponent>(memberEntity);
}
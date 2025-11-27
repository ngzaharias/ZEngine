#include "GameClientPCH.h"
#include "GameClient/ContainerMemberSystem.h"

#include "Core/Algorithms.h"
#include "Core/Optional.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/ContainerComponents.h"

void container::MemberSystem::Update(World& world, const GameTime& gameTime)
{
	ProcessAddRequests(world);
	ProcessMoveRequests(world);
	ProcessRemoveRequests(world);
}

void container::MemberSystem::ProcessAddRequests(World& world)
{
	const auto& changesComponent = world.ReadSingleton<container::StorageChangesSingleton>();
	for (const MemberAdded& data : changesComponent.m_MemberAdded)
	{
		auto& memberComponent = world.AddComponent<container::MemberComponent>(data.m_Member);
		memberComponent.m_Storage = data.m_Storage;
		memberComponent.m_Count = data.m_Count;
		memberComponent.m_GridX = data.m_GridX;
		memberComponent.m_GridY = data.m_GridY;
		memberComponent.m_Type = data.m_Type;
	}
}

void container::MemberSystem::ProcessMoveRequests(World& world)
{
	const auto& changesComponent = world.ReadSingleton<container::StorageChangesSingleton>();
	for (const MemberMoved& data : changesComponent.m_MemberMoved)
	{
		auto& memberComponent = world.WriteComponent<container::MemberComponent>(data.m_Member);
		memberComponent.m_Storage = data.m_Storage;
	}
}

void container::MemberSystem::ProcessRemoveRequests(World& world)
{
	const auto& changesComponent = world.ReadSingleton<container::StorageChangesSingleton>();

	// gather remove requests to handle duplicates
	Set<ecs::Entity> removeRequests;
	for (const MemberRemoved& data : changesComponent.m_MemberRemoved)
		removeRequests.Add(data.m_Member);

	for (const StorageChange& data : changesComponent.m_StorageDestroyed)
	{
		const auto& storageComponent = world.ReadComponent<container::StorageComponent>(data.m_Storage);
		for (const ecs::Entity& memberEntity : storageComponent.m_Members)
			removeRequests.Add(memberEntity);
	}

	// process requests, no safety checks as that indicates an error in the system
	for (const ecs::Entity& memberEntity : removeRequests)
		world.RemoveComponent<container::MemberComponent>(memberEntity);

}
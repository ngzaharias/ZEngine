#include "InventoryPCH.h"
#include "Inventory/InventoryMemberSystem.h"

#include "Core/Algorithms.h"
#include "Core/Optional.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Inventory/InventoryMemberComponent.h"
#include "Inventory/InventoryStorageChangesComponent.h"
#include "Inventory/InventoryStorageComponent.h"

void inventory::MemberSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ProcessAddRequests(world);
	ProcessMoveRequests(world);
	ProcessRemoveRequests(world);
}

void inventory::MemberSystem::ProcessAddRequests(World& world)
{
	const auto& changesComponent = world.ReadComponent<inventory::StorageChangesComponent>();
	for (const MemberAdded& data : changesComponent.m_MemberAdded)
	{
		auto& memberComponent = world.AddComponent<inventory::MemberComponent>(data.m_Member);
		memberComponent.m_Storage = data.m_Storage;
		memberComponent.m_Count = data.m_Count;
		memberComponent.m_GridX = data.m_GridX;
		memberComponent.m_GridY = data.m_GridY;
		memberComponent.m_Type = data.m_Type;
	}
}

void inventory::MemberSystem::ProcessMoveRequests(World& world)
{
	const auto& changesComponent = world.ReadComponent<inventory::StorageChangesComponent>();
	for (const MemberMoved& data : changesComponent.m_MemberMoved)
	{
		auto& memberComponent = world.WriteComponent<inventory::MemberComponent>(data.m_Member);
		memberComponent.m_Storage = data.m_Storage;
	}
}

void inventory::MemberSystem::ProcessRemoveRequests(World& world)
{
	const auto& changesComponent = world.ReadComponent<inventory::StorageChangesComponent>();

	// gather remove requests to handle duplicates
	Set<ecs::Entity> removeRequests;
	for (const MemberRemoved& data : changesComponent.m_MemberRemoved)
		removeRequests.Add(data.m_Member);

	for (const StorageChange& data : changesComponent.m_StorageDestroyed)
	{
		const auto& storageComponent = world.ReadComponent<inventory::StorageComponent>(data.m_Storage);
		for (const ecs::Entity& memberEntity : storageComponent.m_Members)
			removeRequests.Add(memberEntity);
	}

	// process requests, no safety checks as that indicates an error in the system
	for (const ecs::Entity& memberEntity : removeRequests)
		world.RemoveComponent<inventory::MemberComponent>(memberEntity);

}
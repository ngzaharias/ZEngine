#include "ServerInventoryPCH.h"
#include "ServerInventory/InventoryMemberSystem.h"

#include "Core/Algorithms.h"
#include "Core/Optional.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "ServerInventory/InventoryMemberComponent.h"
#include "ServerInventory/InventoryStorageChangesStaticComponent.h"
#include "ServerInventory/InventoryStorageComponent.h"

void server::inventory::MemberSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ProcessAddRequests(world);
	ProcessMoveRequests(world);
	ProcessRemoveRequests(world);
}

void server::inventory::MemberSystem::ProcessAddRequests(World& world)
{
	const auto& changesComponent = world.ReadComponent<server::inventory::StorageChangesStaticComponent>();
	for (const MemberAdded& data : changesComponent.m_MemberAdded)
	{
		auto& memberComponent = world.AddComponent<server::inventory::MemberComponent>(data.m_Member);
		memberComponent.m_Storage = data.m_Storage;
		memberComponent.m_Count = data.m_Count;
		memberComponent.m_GridX = data.m_GridX;
		memberComponent.m_GridY = data.m_GridY;
		memberComponent.m_Type = data.m_Type;
	}
}

void server::inventory::MemberSystem::ProcessMoveRequests(World& world)
{
	const auto& changesComponent = world.ReadComponent<server::inventory::StorageChangesStaticComponent>();
	for (const MemberMoved& data : changesComponent.m_MemberMoved)
	{
		auto& memberComponent = world.WriteComponent<server::inventory::MemberComponent>(data.m_Member);
		memberComponent.m_Storage = data.m_Storage;
	}
}

void server::inventory::MemberSystem::ProcessRemoveRequests(World& world)
{
	const auto& changesComponent = world.ReadComponent<server::inventory::StorageChangesStaticComponent>();

	// gather remove requests to handle duplicates
	Set<ecs::Entity> removeRequests;
	for (const MemberRemoved& data : changesComponent.m_MemberRemoved)
		removeRequests.Add(data.m_Member);

	for (const StorageChange& data : changesComponent.m_StorageDestroyed)
	{
		const auto& storageComponent = world.ReadComponent<server::inventory::StorageComponent>(data.m_Storage);
		for (const ecs::Entity& memberEntity : storageComponent.m_Members)
			removeRequests.Add(memberEntity);
	}

	// process requests, no safety checks as that indicates an error in the system
	for (const ecs::Entity& memberEntity : removeRequests)
		world.RemoveComponent<server::inventory::MemberComponent>(memberEntity);

}
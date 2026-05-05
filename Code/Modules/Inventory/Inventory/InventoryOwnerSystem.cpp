#include "InventoryPCH.h"
#include "Inventory/InventoryOwnerSystem.h"

#include "Core/Algorithms.h"
#include "Core/Optional.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Inventory/InventoryOwnerComponent.h"
#include "Inventory/InventoryStorageChangesComponent.h"

void inventory::OwnerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ProcessStorageChanges(world);
}

void inventory::OwnerSystem::ProcessStorageChanges(World& world)
{
	const auto& storageChangesComponent = world.ReadComponent<inventory::StorageChangesComponent>();

	Map<ecs::Entity, Array<const StorageChange*>> requests;
	for (const StorageChange& data : storageChangesComponent.m_StorageCreated)
		requests[data.m_Owner].Append(&data);

	for (auto&& [ownerEntity, createRequests] : requests)
	{
		if (ownerEntity.IsUnassigned())
			continue;

		auto& ownerComponent = world.HasComponent<inventory::OwnerComponent>(ownerEntity)
			? world.WriteComponent<inventory::OwnerComponent>(ownerEntity)
			: world.AddComponent<inventory::OwnerComponent>(ownerEntity);

		for (const StorageChange* data : createRequests)
			ownerComponent.m_Storages.Set(data->m_Type, data->m_Storage);
	}

	// gather remove requests to handle duplicates
	Set<ecs::Entity> removeRequests;
	for (const StorageChange& data : storageChangesComponent.m_StorageDestroyed)
	{
		// storage owner is allowed to be unassigned
		if (data.m_Owner.IsUnassigned())
			continue;

		auto& ownerComponent = world.WriteComponent<inventory::OwnerComponent>(data.m_Owner);
		ownerComponent.m_Storages.Remove(data.m_Type);

		if (ownerComponent.m_Storages.IsEmpty())
			removeRequests.Add(data.m_Owner);
	}

	// process requests, no safety checks as that indicates an error in the system
	for (const ecs::Entity& ownerEntity : removeRequests)
		world.RemoveComponent<inventory::OwnerComponent>(ownerEntity);
}
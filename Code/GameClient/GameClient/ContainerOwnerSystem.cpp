#include "GameClientPCH.h"
#include "GameClient/ContainerOwnerSystem.h"

#include <Core/Algorithms.h>
#include <Core/Nullable.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "GameClient/ContainerComponents.h"

void container::OwnerSystem::Update(World& world, const GameTime& gameTime)
{
	ProcessStorageChanges(world);
}

void container::OwnerSystem::ProcessStorageChanges(World& world)
{
	const auto& storageChangesComponent = world.GetSingleton<const container::StorageChangesComponent>();

	Map<ecs::Entity, Array<const StorageChange*>> requests;
	for (const StorageChange& data : storageChangesComponent.m_Created)
		requests[data.m_Owner].Append(&data);

	for (auto&& [ownerEntity, createRequests] : requests)
	{
		if (ownerEntity.IsUnassigned())
			continue;

		auto& ownerComponent = world.HasComponent<container::OwnerComponent>(ownerEntity)
			? world.GetComponent<container::OwnerComponent>(ownerEntity)
			: world.AddComponent<container::OwnerComponent>(ownerEntity);

		for (const StorageChange* data : createRequests)
			ownerComponent.m_Storages.Set(data->m_Type, data->m_Storage);
	}

	for (const StorageChange& data : storageChangesComponent.m_Destroyed)
	{
		if (data.m_Owner.IsUnassigned())
			continue;

		auto& ownerComponent = world.GetComponent<container::OwnerComponent>(data.m_Owner);
		ownerComponent.m_Storages.Remove(data.m_Type);

		if (ownerComponent.m_Storages.IsEmpty())
			world.RemoveComponent<container::OwnerComponent>(data.m_Owner);
	}
}
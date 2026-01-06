#include "EnginePCH.h"
#include "Engine/ReplicationHost.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "Engine/NetworkManager.h"
#include "Engine/ReplicationComponent.h"
#include "Engine/ComponentRegistry.h"
#include "Network/Host.h"
#include "Network/Messages.h"
#include "Network/Types.h"

namespace
{
	net::Entity ToNetEntity(const ecs::Entity& entity)
	{
		return net::Entity(entity.m_Value);
	}
}

net::ReplicationHost::ReplicationHost(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
	, m_PeerReplicationData()
{
}

void net::ReplicationHost::Initialise()
{
	PROFILE_FUNCTION();
}

void net::ReplicationHost::Shutdown()
{
	PROFILE_FUNCTION();
}

void net::ReplicationHost::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& networkManager = m_EntityWorld.ReadResource<eng::NetworkManager>();
	const auto& componentRegistry = networkManager.GetComponentRegistry();

	for (auto&& [peerId, replicationData] : m_PeerReplicationData)
	{
		const auto& registry = m_EntityWorld.m_QueryRegistry;
		const auto& entities = m_EntityWorld.m_EntityStorage.m_AliveEntities;

		auto& replicated = replicationData.m_EntitiesReplicated;
		auto& toCreate = replicationData.m_EntitiesToCreate;
		auto& toDestroy = replicationData.m_EntitiesToDestroy;

		for (const ecs::Entity& entity : toCreate)
		{
			CreateEntity(peerId, entity);

			const ecs::ComponentMask& componentMask = entities.Get(entity);
			for (ecs::ComponentId worldId = 0; worldId < ecs::COMPONENTS_MAX; ++worldId)
			{
				if (!componentMask.Has(worldId))
					continue;

				const auto find = componentRegistry.m_WorldToLocal.Find(worldId);
				if (find == componentRegistry.m_WorldToLocal.end())
					continue;

				const ecs::ComponentId localId = find->second;
				const net::ComponentEntry& entry = componentRegistry.m_Entries.Get(localId);

				AddComponent(peerId, entity, entry);
			}
		}

		for (const ecs::Entity& entity : toDestroy)
		{
			DestroyEntity(peerId, entity);
			replicated.Remove(entity);
		}

		for (auto&& [localId, entry] : componentRegistry.m_Entries)
		{
			const ecs::QueryGroup& toAdd = registry.GetGroup(entry.m_AddedId);
			const ecs::QueryGroup& toRemove = registry.GetGroup(entry.m_RemovedId);
			const ecs::QueryGroup& toUpdate = registry.GetGroup(entry.m_UpdatedId);

			// #note: this only handles components added AFTER an entity was marked for replication
			for (const ecs::Entity& entity : toAdd)
			{
				// #note: components added on the same frame it started replicating
				// will show up in the added query, so they need to ignored
				if (enumerate::Contains(toCreate, entity))
					continue;
				if (!IsReplicated(peerId, entity))
					continue;

				AddComponent(peerId, entity, entry);
			}

			for (const ecs::Entity& entity : toRemove)
			{
				if (!IsReplicated(peerId, entity))
					continue;

				RemoveComponent(peerId, entity, entry);
			}

			for (const ecs::Entity& entity : toUpdate)
			{
				if (enumerate::Contains(toRemove, entity))
					continue;
				if (!IsReplicated(peerId, entity))
					continue;

				UpdateComponent(peerId, entity, entry);
			}
		}

		toCreate.RemoveAll();
		toDestroy.RemoveAll();
	}
}

void net::ReplicationHost::RegisterPeer(const PeerId& peerId)
{
	Z_PANIC(!IsRegistered(peerId), "Peer is already registered!");

	PeerReplicationData peerReplicationData;
	peerReplicationData.m_PeerId = peerId;
	m_PeerReplicationData.Set(peerId, std::move(peerReplicationData));
}

void net::ReplicationHost::UnregisterPeer(const PeerId& peerId)
{
	Z_PANIC(IsRegistered(peerId), "Peer isn't registered!");
	m_PeerReplicationData.Remove(peerId);
}

void net::ReplicationHost::StartReplicateToPeer(const PeerId& peerId, const ecs::Entity& entity)
{
	Z_PANIC(IsRegistered(peerId), "Peer isn't registered!");

	auto& peerReplicationData = m_PeerReplicationData.Get(peerId);
	if (peerReplicationData.m_EntitiesToDestroy.Contains(entity))
	{
		peerReplicationData.m_EntitiesToDestroy.Remove(entity);
	}
	else
	{
		Z_PANIC(!IsReplicated(peerId, entity), "Trying to start replication of an entity that is already replicated!");
		peerReplicationData.m_EntitiesToCreate.Add(entity);
		peerReplicationData.m_EntitiesReplicated.Add(entity);

		// #todo: will crash when replicated to two peers in the same frame
		auto& peerComponent = !m_EntityWorld.HasComponent<net::ReplicationComponent>(entity)
			? m_EntityWorld.AddComponent<net::ReplicationComponent>(entity)
			: m_EntityWorld.WriteComponent<net::ReplicationComponent>(entity);
		peerComponent.m_Peers.Add(peerId);
	}
}

void net::ReplicationHost::StopReplicateToPeer(const PeerId& peerId, const ecs::Entity& entity)
{
	Z_PANIC(IsRegistered(peerId), "Peer isn't registered!");

	auto& peerReplicationData = m_PeerReplicationData.Get(peerId);
	if (peerReplicationData.m_EntitiesToCreate.Contains(entity))
	{
		peerReplicationData.m_EntitiesToCreate.Remove(entity);
	}
	else
	{
		Z_PANIC(IsReplicated(peerId, entity), "Trying to stop replication of an entity that isn't replicated!");
		peerReplicationData.m_EntitiesToDestroy.Add(entity);

		auto& peerComponent = m_EntityWorld.WriteComponent<net::ReplicationComponent>(entity);
		peerComponent.m_Peers.Remove(peerId);
		if (peerComponent.m_Peers.IsEmpty())
			m_EntityWorld.RemoveComponent<net::ReplicationComponent>(entity);
	}
}

bool net::ReplicationHost::IsRegistered(const PeerId& peerId)
{
	return m_PeerReplicationData.Contains(peerId);
}

bool net::ReplicationHost::IsReplicated(const PeerId& peerId, const ecs::Entity& entity)
{
	Z_PANIC(IsRegistered(peerId), "Peer isn't registered!");

	const auto& peerReplicationData = m_PeerReplicationData.Get(peerId);
	return peerReplicationData.m_EntitiesReplicated.Contains(entity);
}

void net::ReplicationHost::CreateEntity(const net::PeerId& peerId, const ecs::Entity& entity)
{
	auto& networkManager = m_EntityWorld.WriteResource<eng::NetworkManager>();
	auto& host = networkManager.GetHost();

	auto* message = host.CreateMessage<CreateEntityMessage>(peerId, EMessage::CreateEntity);
	message->m_Entity = entity.m_Value;
	host.SendMessage(peerId, message);
}

void net::ReplicationHost::DestroyEntity(const net::PeerId& peerId, const ecs::Entity& entity)
{
	auto& networkManager = m_EntityWorld.WriteResource<eng::NetworkManager>();
	auto& host = networkManager.GetHost();

	auto* message = host.CreateMessage<DestroyEntityMessage>(peerId, EMessage::DestroyEntity);
	message->m_Entity = entity.m_Value;

	host.SendMessage(peerId, message);
}

void net::ReplicationHost::AddComponent(const net::PeerId& peerId, const ecs::Entity& entity, const net::ComponentEntry& entry)
{
	auto& networkManager = m_EntityWorld.WriteResource<eng::NetworkManager>();
	auto& host = networkManager.GetHost();

	auto* message = host.CreateMessage<AddComponentMessage>(peerId, EMessage::AddComponent);
	message->m_Entity = ToNetEntity(entity);
	message->m_ComponentId = entry.m_ComponentId;
	entry.m_Read(m_EntityWorld, entity, message->m_Data);

	host.SendMessage(peerId, message);
}

void net::ReplicationHost::RemoveComponent(const net::PeerId& peerId, const ecs::Entity& entity, const net::ComponentEntry& entry)
{
	auto& networkManager = m_EntityWorld.WriteResource<eng::NetworkManager>();
	auto& host = networkManager.GetHost();

	auto* message = host.CreateMessage<RemoveComponentMessage>(peerId, EMessage::RemoveComponent);
	message->m_Entity = ToNetEntity(entity);
	message->m_ComponentId = entry.m_ComponentId;

	host.SendMessage(peerId, message);
}

void net::ReplicationHost::UpdateComponent(const net::PeerId& peerId, const ecs::Entity& entity, const net::ComponentEntry& entry)
{
	auto& networkManager = m_EntityWorld.WriteResource<eng::NetworkManager>();
	auto& host = networkManager.GetHost();

	auto* message = host.CreateMessage<UpdateComponentMessage>(peerId, EMessage::UpdateComponent);
	message->m_Entity = ToNetEntity(entity);
	message->m_ComponentId = entry.m_ComponentId;
	entry.m_Read(m_EntityWorld, entity, message->m_Data);

	host.SendMessage(peerId, message);
}

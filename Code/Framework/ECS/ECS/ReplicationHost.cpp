#include "ECS/ReplicationHost.h"

#include "Core/Algorithms.h"
#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/Messages.h"
#include "ECS/ReplicationComponent.h"
#include "ECS/TypeRegistry.h"
#include "ECS/WorldView.h"
#include "Network/Host.h"

namespace
{
	net::Entity ToNetEntity(const ecs::Entity& entity)
	{
		return net::Entity(entity.m_Value);
	}
}

ecs::ReplicationHost::ReplicationHost(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void ecs::ReplicationHost::Initialise()
{
	PROFILE_FUNCTION();

	auto& host = m_EntityWorld.WriteResource<net::Host>();
	m_Collection =
	{
		host.m_OnPeerConnected.Connect(*this, &ecs::ReplicationHost::OnPeerConnected),
		host.m_OnPeerDisconnected.Connect(*this, &ecs::ReplicationHost::OnPeerDisconnected),
		host.m_OnProcessMessages.Connect(*this, &ecs::ReplicationHost::OnProcessMessages),
	};
}

void ecs::ReplicationHost::Shutdown()
{
	PROFILE_FUNCTION();
}

void ecs::ReplicationHost::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ProcessEntities();
	ProcessEvents();
}

void ecs::ReplicationHost::RegisterPeer(const net::PeerId& peerId)
{
	Z_PANIC(!IsRegistered(peerId), "Peer is already registered!");

	ReplicationData replicationData;
	replicationData.m_PeerId = peerId;
	m_ReplicationMap.Insert(peerId, std::move(replicationData));
}

void ecs::ReplicationHost::UnregisterPeer(const net::PeerId& peerId)
{
	Z_PANIC(IsRegistered(peerId), "Peer isn't registered!");
	m_ReplicationMap.Remove(peerId);
}

void ecs::ReplicationHost::StartReplicate(const net::PeerId& peerId, const ecs::Entity& entity)
{
	Z_PANIC(!IsReplicated(peerId, entity), "Trying to start replication of an entity that is already replicated!");

	ReplicationData& replicationData = m_ReplicationMap.Get(peerId);
	replicationData.m_ToCreate.Add(entity);
}

void ecs::ReplicationHost::StopReplicate(const net::PeerId& peerId, const ecs::Entity& entity)
{
	Z_PANIC(IsReplicated(peerId, entity), "Trying to stop replication of an entity that isn't replicated!");

	ReplicationData& replicationData = m_ReplicationMap.Get(peerId);
	replicationData.m_ToDestroy.Add(entity);
}

bool ecs::ReplicationHost::IsRegistered(const net::PeerId& peerId)
{
	return m_ReplicationMap.Contains(peerId);
}

bool ecs::ReplicationHost::IsReplicated(const net::PeerId& peerId, const ecs::Entity& entity)
{
	const ReplicationData& replicationData = m_ReplicationMap.Get(peerId);
	return replicationData.m_Replicated.Contains(entity);
}

// #todo: entities created/destroyed
void ecs::ReplicationHost::ProcessEntities()
{
	PROFILE_FUNCTION();

	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	const auto& queries = m_EntityWorld.m_QueryRegistry;

	for (auto&& [peerId, replicationData] : m_ReplicationMap)
	{
		// entities that were removed from replication this frame
		for (const ecs::Entity& entity : replicationData.m_ToDestroy)
		{
			EntityDestroy(peerId, entity);
		}

		for (ecs::ComponentId typeId = 0; typeId < ecs::COMPONENTS_MAX; ++typeId)
		{
			const ecs::TypeComponent* entry = registry.TryTypeComponent(typeId);
			if (!entry || !entry->m_IsReplicated)
				continue;

			Set<ecs::Entity> toAdd, toUpdate, toRemove;
			const Set<ecs::Entity>& replicated = replicationData.m_Replicated;
			enumerate::Intersection(replicated, queries.GetGroup(entry->m_AddedId), toAdd);
			enumerate::Intersection(replicated, queries.GetGroup(entry->m_UpdatedId), toUpdate);
			enumerate::Intersection(replicated, queries.GetGroup(entry->m_RemovedId), toRemove);

			// #note: this only handles component AFTER an entity was marked for replication
			for (const ecs::Entity& entity : toAdd)
				ComponentAdd(peerId, entity, *entry);
			for (const ecs::Entity& entity : toUpdate)
				ComponentUpdate(peerId, entity, *entry);
			for (const ecs::Entity& entity : toRemove)
				ComponentRemove(peerId, entity, *entry);
		}

		// entities that were added to replication this frame
		const Set<ecs::Entity>& created = replicationData.m_ToCreate;
		for (const ecs::Entity& entity : created)
		{
			EntityCreate(peerId, entity);
		}
		// ...and their components
		for (ecs::ComponentId typeId = 0; typeId < ecs::COMPONENTS_MAX; ++typeId)
		{
			const ecs::TypeComponent* entry = registry.TryTypeComponent(typeId);
			if (!entry || !entry->m_IsReplicated)
				continue;

			Set<ecs::Entity> toAdd;
			enumerate::Intersection(created, queries.GetGroup(entry->m_IncludeId), toAdd);

			for (const ecs::Entity& entity : toAdd)
				ComponentAdd(peerId, entity, *entry);
		}

		replicationData.m_ToCreate.RemoveAll();
		replicationData.m_ToDestroy.RemoveAll();
	}
}

void ecs::ReplicationHost::ProcessEvents()
{
	PROFILE_FUNCTION();

	MemBuffer data;
	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	const auto& storage = m_EntityWorld.m_EventStorage;

	for (auto&& [peerId, replicationData] : m_ReplicationMap)
	{
		// #hack: we iterate the keys of the remote buffer but fetch from the local buffer
		const ecs::EventBuffer& mainBuffer = storage.GetMainBuffer();
		const ecs::EventBuffer& syncBuffer = storage.GetSyncBuffer();
		for (const ecs::EventId& typeId : syncBuffer.GetAll().GetKeys())
		{
			const ecs::IEventContainer& container = mainBuffer.GetAt(typeId);
			const int32 count = container.GetCount();
			for (int32 i = 0; i < count; ++i)
			{
				data.Reset();
				container.ReadAt(data, i);
				EventAdd(peerId, typeId, data);
			}
		}
	}
}

void ecs::ReplicationHost::OnPeerConnected(const net::PeerId& peerId)
{
	RegisterPeer(peerId);
}

void ecs::ReplicationHost::OnPeerDisconnected(const net::PeerId& peerId)
{
	UnregisterPeer(peerId);
}

void ecs::ReplicationHost::OnProcessMessages(const Array<const net::Message*>& messages)
{
	PROFILE_FUNCTION();

	for (const net::Message* message : messages)
	{
		const ecs::EMessage type = static_cast<ecs::EMessage>(message->m_Type);
		switch (type)
		{
		case ecs::EMessage::EntityCreate:
		case ecs::EMessage::EntityDestroy:
		case ecs::EMessage::ComponentAdd:
		case ecs::EMessage::ComponentRemove:
		case ecs::EMessage::ComponentUpdate:
		{
			Z_PANIC(false, "Unsupported!");
		} break;

		case ecs::EMessage::EventAdd:
			OnEventAdd(static_cast<const ecs::EventAddMessage*>(message));
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Entity

void ecs::ReplicationHost::EntityCreate(const net::PeerId& peerId, const ecs::Entity& entity)
{
	ReplicationData& replicationData = m_ReplicationMap.Get(peerId);
	replicationData.m_Replicated.Add(entity);

	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::EntityCreateMessage>(ecs::EMessage::EntityCreate);
	message->m_Entity = entity.m_Value;

	host.SendMessage(peerId, message);
	host.ReleaseMessage(message);
}

void ecs::ReplicationHost::EntityDestroy(const net::PeerId& peerId, const ecs::Entity& entity)
{
	ReplicationData& replicationData = m_ReplicationMap.Get(peerId);
	replicationData.m_Replicated.Remove(entity);

	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::EntityDestroyMessage>(ecs::EMessage::EntityDestroy);
	message->m_Entity = entity.m_Value;

	host.SendMessage(peerId, message);
	host.ReleaseMessage(message);
}

//////////////////////////////////////////////////////////////////////////
// Component

void ecs::ReplicationHost::ComponentAdd(const net::PeerId& peerId, const ecs::Entity& entity, const ecs::TypeComponent& entry)
{
	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::ComponentAddMessage>(ecs::EMessage::ComponentAdd);
	message->m_Entity = ToNetEntity(entity);
	message->m_TypeId = entry.m_TypeId;
	entry.m_Read(m_EntityWorld.m_EntityStorage, entity, message->m_Data);

	host.SendMessage(peerId, message);
	host.ReleaseMessage(message);
}

void ecs::ReplicationHost::ComponentUpdate(const net::PeerId& peerId, const ecs::Entity& entity, const ecs::TypeComponent& entry)
{
	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::ComponentUpdateMessage>(ecs::EMessage::ComponentUpdate);
	message->m_Entity = ToNetEntity(entity);
	message->m_TypeId = entry.m_TypeId;
	entry.m_Read(m_EntityWorld.m_EntityStorage, entity, message->m_Data);

	host.SendMessage(peerId, message);
	host.ReleaseMessage(message);
}

void ecs::ReplicationHost::ComponentRemove(const net::PeerId& peerId, const ecs::Entity& entity, const ecs::TypeComponent& entry)
{
	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::ComponentRemoveMessage>(ecs::EMessage::ComponentRemove);
	message->m_Entity = ToNetEntity(entity);
	message->m_TypeId = entry.m_TypeId;

	host.SendMessage(peerId, message);
	host.ReleaseMessage(message);
}

//////////////////////////////////////////////////////////////////////////
// Event

void ecs::ReplicationHost::EventAdd(const net::PeerId& peerId, const ecs::EventId typeId, const MemBuffer& data)
{
	auto& host = m_EntityWorld.WriteResource<net::Host>();
	auto* message = host.RequestMessage<ecs::EventAddMessage>(ecs::EMessage::EventAdd);
	message->m_TypeId = typeId;
	message->m_Data.Write(data);

	host.SendMessage(peerId, message);
	host.ReleaseMessage(message);
}

void ecs::ReplicationHost::OnEventAdd(const ecs::EventAddMessage* message)
{
	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	ecs::EventStorage& storage = m_EntityWorld.m_EventStorage;
	ecs::EventBuffer& buffer = storage.GetSyncBuffer();

	MemBuffer data;
	message->m_Data.Read(data);
	registry.AddEvent(buffer, message->m_TypeId, data);
}
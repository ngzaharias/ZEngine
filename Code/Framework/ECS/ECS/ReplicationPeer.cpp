#include "ECS/ReplicationPeer.h"

#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/Messages.h"
#include "ECS/TypeRegistry.h"
#include "Network/Peer.h"

ecs::ReplicationPeer::ReplicationPeer(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void ecs::ReplicationPeer::Initialise()
{
	PROFILE_FUNCTION();

	auto& peer = m_EntityWorld.WriteResource<net::Peer>();
	m_Collection =
	{
		peer.m_OnProcessMessages.Connect(*this, &ecs::ReplicationPeer::OnProcessMessages),
	};
}

void ecs::ReplicationPeer::Shutdown()
{
	PROFILE_FUNCTION();

	m_Collection.Disconnect();
}

void ecs::ReplicationPeer::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& peer = m_EntityWorld.ReadResource<net::Peer>();
	if (peer.IsConnected())
	{
		ProcessEvents();

		for (auto&& [peerEntity, hostEntity] : m_ToDestroy)
		{
			m_HostToPeer.Remove(hostEntity);
			m_PeerToHost.Remove(peerEntity);
		}
		m_ToDestroy.RemoveAll();
	}
	else if (!m_PeerToHost.IsEmpty())
	{
		for (auto&& [entity, nentity] : m_PeerToHost)
			m_EntityWorld.DestroyEntity(entity);

		m_HostToPeer.RemoveAll();
		m_PeerToHost.RemoveAll();
	}
}

void ecs::ReplicationPeer::ProcessEvents()
{
	PROFILE_FUNCTION();

	MemBuffer data;
	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	const auto& storage = m_EntityWorld.m_EventStorage;

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
			EventAdd(typeId, data);
		}
	}
}

void ecs::ReplicationPeer::OnProcessMessages(const Array<const net::Message*>& messages)
{
	PROFILE_FUNCTION();

	for (const net::Message* message : messages)
	{
		const ecs::EMessage type = static_cast<ecs::EMessage>(message->m_Type);
		switch (type)
		{
		case ecs::EMessage::EntityCreate:
			OnEntityCreate(static_cast<const ecs::EntityCreateMessage*>(message));
			break;
		case ecs::EMessage::EntityDestroy:
			OnEntityDestroy(static_cast<const ecs::EntityDestroyMessage*>(message));
			break;

		case ecs::EMessage::ComponentAdd:
			OnComponentAdd(static_cast<const ecs::ComponentAddMessage*>(message));
			break;
		case ecs::EMessage::ComponentUpdate:
			OnComponentUpdate(static_cast<const ecs::ComponentUpdateMessage*>(message));
			break;
		case ecs::EMessage::ComponentRemove:
			OnComponentRemove(static_cast<const ecs::ComponentRemoveMessage*>(message));
			break;

		case ecs::EMessage::EventAdd:
			OnEventAdd(static_cast<const ecs::EventAddMessage*>(message));
			break;

		case ecs::EMessage::SingletonUpdate:
			OnSingletonUpdate(static_cast<const ecs::SingletonUpdateMessage*>(message));
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Entity

void ecs::ReplicationPeer::OnEntityCreate(const ecs::EntityCreateMessage* message)
{
	ecs::EntityBuffer& buffer = m_EntityWorld.m_EntityStorage.GetEntityBuffer();
	const net::Entity& hostEntity = message->m_Entity;
	const auto findEntity = m_HostToPeer.Find(hostEntity);
	if (findEntity != m_HostToPeer.end())
	{
		const ecs::Entity& peerEntity = findEntity->second;
		buffer.m_EntityChanges[peerEntity].m_IsDestroy = false;
		m_ToDestroy.Remove(peerEntity);
	}
	else
	{
		const ecs::Entity peerEntity = buffer.CreateEntity();
		m_HostToPeer[hostEntity] = peerEntity;
		m_PeerToHost[peerEntity] = hostEntity;
	}
}

void ecs::ReplicationPeer::OnEntityDestroy(const ecs::EntityDestroyMessage* message)
{
	Z_PANIC(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity {} doesn't exist on peer!", message->m_Entity.m_Value);

	const net::Entity& hostEntity = message->m_Entity;
	const ecs::Entity& peerEntity = m_HostToPeer[hostEntity];

	ecs::EntityBuffer& buffer = m_EntityWorld.m_EntityStorage.GetEntityBuffer();
	buffer.DestroyEntity(peerEntity);
	m_ToDestroy.Set(peerEntity, hostEntity);
}

//////////////////////////////////////////////////////////////////////////
// Component

void ecs::ReplicationPeer::OnComponentAdd(const ecs::ComponentAddMessage* message)
{
	Z_PANIC(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity {} doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	ecs::EntityStorage& storage = m_EntityWorld.m_EntityStorage;
	ecs::EntityBuffer& buffer = storage.GetEntityBuffer();

	const ecs::Entity& entity = m_HostToPeer.Get(message->m_Entity);
	ecs::EntityChange& change = buffer.m_EntityChanges[entity];
	if (!change.m_Removed.Has(message->m_TypeId))
	{
		change.m_Added.Raise(message->m_TypeId);
		registry.AddComponent(storage, message->m_TypeId, entity, message->m_Data);
	}

	change.m_Removed.Clear(message->m_TypeId);
}

void ecs::ReplicationPeer::OnComponentUpdate(const ecs::ComponentUpdateMessage* message)
{
	Z_PANIC(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity {} doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	ecs::EntityStorage& storage = m_EntityWorld.m_EntityStorage;
	ecs::EntityBuffer& buffer = storage.GetEntityBuffer();

	const ecs::Entity& entity = m_HostToPeer.Get(message->m_Entity);
	registry.UpdateComponent(storage, message->m_TypeId, entity, message->m_Data);

	ecs::EntityChange& change = buffer.m_EntityChanges[entity];
	change.m_Updated.Raise(message->m_TypeId);
}

void ecs::ReplicationPeer::OnComponentRemove(const ecs::ComponentRemoveMessage* message)
{
	Z_PANIC(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity {} doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	ecs::EntityStorage& storage = m_EntityWorld.m_EntityStorage;
	ecs::EntityBuffer& buffer = storage.GetEntityBuffer();

	const ecs::Entity& entity = m_HostToPeer.Get(message->m_Entity);
	registry.RemoveComponent(storage, message->m_TypeId, entity);

	ecs::EntityChange& change = buffer.m_EntityChanges[entity];
	change.m_Added.Clear(message->m_TypeId);
	change.m_Updated.Clear(message->m_TypeId);
	change.m_Removed.Raise(message->m_TypeId);
}

//////////////////////////////////////////////////////////////////////////
// Event

void ecs::ReplicationPeer::EventAdd(const ecs::EventId typeId, const MemBuffer& data)
{
	auto& peer = m_EntityWorld.WriteResource<net::Peer>();

	auto* message = peer.RequestMessage<ecs::EventAddMessage>(ecs::EMessage::EventAdd);
	message->m_TypeId = typeId;
	message->m_Data.Write(data);

	peer.SendMessage(message);
	peer.ReleaseMessage(message);
}

void ecs::ReplicationPeer::OnEventAdd(const ecs::EventAddMessage* message)
{
	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	ecs::EventStorage& storage = m_EntityWorld.m_EventStorage;
	ecs::EventBuffer& buffer = storage.GetSyncBuffer();

	MemBuffer data;
	message->m_Data.Read(data);
	registry.AddEvent(buffer, message->m_TypeId, data);
}

//////////////////////////////////////////////////////////////////////////
// Singleton

void ecs::ReplicationPeer::OnSingletonUpdate(const ecs::SingletonUpdateMessage* message)
{
	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	registry.UpdateSingleton(m_EntityWorld, message->m_TypeId, message->m_Data);
}
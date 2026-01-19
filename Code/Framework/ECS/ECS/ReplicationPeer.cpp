#include "ECS/ReplicationPeer.h"

#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/Messages.h"
#include "ECS/TypeRegistry.h"
#include "Network/NetworkManager.h"
#include "Network/Peer.h"

ecs::ReplicationPeer::ReplicationPeer(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void ecs::ReplicationPeer::Initialise()
{
	PROFILE_FUNCTION();

	auto& manager = m_EntityWorld.WriteResource<net::NetworkManager>();
	auto& peer = manager.GetPeer();

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

	MemBuffer data;
	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	const auto& storage = m_EntityWorld.m_EventStorage;

	// #hack: we iterate the keys of the remote buffer but fetch from the local buffer
	for (const ecs::EventId& typeId : storage.m_BufferRemoteCurr.GetAll().GetKeys())
	{
		const ecs::IEventContainer& container = storage.m_BufferLocalCurr.GetAt(typeId);
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
	for (const net::Message* message : messages)
	{
		switch (static_cast<ecs::EMessage>(message->m_Type))
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
		case ecs::EMessage::ComponentRemove:
			OnComponentRemove(static_cast<const ecs::ComponentRemoveMessage*>(message));
			break;
		case ecs::EMessage::ComponentUpdate:
			OnComponentUpdate(static_cast<const ecs::ComponentUpdateMessage*>(message));
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
	Z_PANIC(!enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity {} has already been added on peer!", message->m_Entity.m_Value);

	const net::Entity hostEntity = message->m_Entity;
	const ecs::Entity peerEntity = m_EntityWorld.CreateEntity();

	m_HostToPeer[hostEntity] = peerEntity;
	m_PeerToHost[peerEntity] = hostEntity;
}

void ecs::ReplicationPeer::OnEntityDestroy(const ecs::EntityDestroyMessage* message)
{
	Z_PANIC(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity {} doesn't exist on peer!", message->m_Entity.m_Value);

	const net::Entity& hostEntity = message->m_Entity;
	const ecs::Entity& peerEntity = m_HostToPeer[hostEntity];

	m_EntityWorld.DestroyEntity(peerEntity);
	m_PeerToHost.Remove(peerEntity);
	m_HostToPeer.Remove(hostEntity);
}

//////////////////////////////////////////////////////////////////////////
// Component

void ecs::ReplicationPeer::OnComponentAdd(const ecs::ComponentAddMessage* message)
{
	Z_PANIC(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity {} doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	const ecs::Entity& entity = m_HostToPeer[message->m_Entity];
	registry.AddComponent(m_EntityWorld, message->m_TypeId, entity, message->m_Data);
}

void ecs::ReplicationPeer::OnComponentUpdate(const ecs::ComponentUpdateMessage* message)
{
	Z_PANIC(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity {} doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	const ecs::Entity& entity = m_HostToPeer[message->m_Entity];
	registry.UpdateComponent(m_EntityWorld, message->m_TypeId, entity, message->m_Data);
}

void ecs::ReplicationPeer::OnComponentRemove(const ecs::ComponentRemoveMessage* message)
{
	Z_PANIC(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity {} doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& registry = m_EntityWorld.ReadResource<ecs::TypeRegistry>();
	const ecs::Entity& entity = m_HostToPeer[message->m_Entity];
	registry.RemoveComponent(m_EntityWorld, message->m_TypeId, entity);
}

//////////////////////////////////////////////////////////////////////////
// Event

void ecs::ReplicationPeer::EventAdd(const ecs::EventId typeId, const MemBuffer& data)
{
	auto& manager = m_EntityWorld.WriteResource<net::NetworkManager>();
	auto& peer = manager.GetPeer();

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
	ecs::EventBuffer& buffer = storage.m_BufferRemoteCurr;

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
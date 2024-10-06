#include "EnginePCH.h"
#include "Engine/ReplicationPeer.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "Engine/ComponentSerializer.h"
#include "Engine/NetworkComponents.h"
#include "Engine/NetworkManager.h"
#include "Engine/ReplicationComponents.h"
#include "Network/Adaptor.h"
#include "Network/Messages.h"
#include "Network/Peer.h"
#include "Network/PeerId.h"

#include <yojimbo/yojimbo.h>

net::ReplicationPeer::ReplicationPeer(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void net::ReplicationPeer::Initialise()
{
	auto& networkManager = m_EntityWorld.WriteResource<eng::NetworkManager>();
	auto& adaptor = networkManager.GetAdaptor();
	auto& peer = networkManager.GetPeer();

	m_Connections =
	{
		adaptor.m_OnServerClientDisconnected.Connect(*this, &net::ReplicationPeer::OnClientDisconnected),
		peer.m_OnProcessMessage.Connect(*this, &net::ReplicationPeer::OnProcessMessage),
	};
}

void net::ReplicationPeer::Shutdown()
{
	auto& networkManager = m_EntityWorld.WriteResource<eng::NetworkManager>();
	auto& adaptor = networkManager.GetAdaptor();
	auto& peer = networkManager.GetPeer();

	adaptor.m_OnServerClientDisconnected.Disconnect(m_Connections[0]);
	peer.m_OnProcessMessage.Disconnect(m_Connections[1]);
	m_Connections.RemoveAll();
}

void net::ReplicationPeer::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// #todo: process messages here instead ?

	if (m_HasDisconnected)
	{
		for (auto&& [peerEntity, hostEntity] : m_PeerToHost)
			m_EntityWorld.DestroyEntity(peerEntity);
		m_HostToPeer.RemoveAll();
		m_PeerToHost.RemoveAll();

		m_HasDisconnected = false;
	}
}

void net::ReplicationPeer::OnClientDisconnected(const net::PeerId& peerId)
{
	m_HasDisconnected = true;
}

void net::ReplicationPeer::OnProcessMessage(const yojimbo::Message* message)
{
	const auto messageType = ValueToEnum<EMessage>(message->GetType());
	switch (messageType)
	{
	case EMessage::CreateEntity:
		OnCreateEntity(reinterpret_cast<const net::CreateEntityMessage*>(message));
		break;
	case EMessage::DestroyEntity:
		OnDestroyEntity(reinterpret_cast<const net::DestroyEntityMessage*>(message));
		break;

	case EMessage::AddComponent:
		OnAddComponent(reinterpret_cast<const net::AddComponentMessage*>(message));
		break;
	case EMessage::RemoveComponent:
		OnRemoveComponent(reinterpret_cast<const net::RemoveComponentMessage*>(message));
		break;
	case EMessage::UpdateComponent:
		OnUpdateComponent(reinterpret_cast<const net::UpdateComponentMessage*>(message));
		break;
	}
}

void net::ReplicationPeer::OnCreateEntity(const net::CreateEntityMessage* message)
{
	Z_LOG(ELog::Network, "Peer: CreateEntityMessage");
	Z_ASSERT(!enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity [{}] has already been added on peer!", message->m_Entity.m_Value);

	const net::Entity hostHandle = message->m_Entity;
	const ecs::Entity peerHandle = m_EntityWorld.CreateEntity();

	m_HostToPeer[hostHandle] = peerHandle;
	m_PeerToHost[peerHandle] = hostHandle;
}

void net::ReplicationPeer::OnDestroyEntity(const net::DestroyEntityMessage* message)
{
	Z_LOG(ELog::Network, "Peer: DestroyEntityMessage");
	Z_ASSERT(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity [{}] doesn't exist on peer!", message->m_Entity.m_Value);

	const net::Entity& hostHandle = message->m_Entity;
	const ecs::Entity& peerHandle = m_HostToPeer[hostHandle];

	m_EntityWorld.DestroyEntity(peerHandle);
	m_PeerToHost.Remove(peerHandle);
	m_HostToPeer.Remove(hostHandle);
}

void net::ReplicationPeer::OnAddComponent(const net::AddComponentMessage* message)
{
	Z_LOG(ELog::Network, "Peer: AddComponentMessage");
	Z_ASSERT(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity [{}] doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& networkManager = m_EntityWorld.ReadResource<eng::NetworkManager>();
	const auto& serializer = networkManager.GetSerializer();

	const ecs::Entity& peerHandle = m_HostToPeer[message->m_Entity];
	const auto& entry = serializer.m_Entries[message->m_ComponentId];
	entry.m_Add(m_EntityWorld, peerHandle, message->m_Data);
}

void net::ReplicationPeer::OnUpdateComponent(const net::UpdateComponentMessage* message)
{
	Z_LOG(ELog::Network, "Peer: UpdateComponentMessage");
	Z_ASSERT(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity [{}] doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& networkManager = m_EntityWorld.ReadResource<eng::NetworkManager>();
	const auto& serializer = networkManager.GetSerializer();

	const ecs::Entity& peerHandle = m_HostToPeer[message->m_Entity];
	const auto& entry = serializer.m_Entries[message->m_ComponentId];
	entry.m_Read(m_EntityWorld, peerHandle, message->m_Data);
}

void net::ReplicationPeer::OnRemoveComponent(const net::RemoveComponentMessage* message)
{
	Z_LOG(ELog::Network, "Peer: RemoveComponentMessage");
	Z_ASSERT(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity [{}] doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& networkManager = m_EntityWorld.ReadResource<eng::NetworkManager>();
	const auto& serializer = networkManager.GetSerializer();

	const ecs::Entity& peerHandle = m_HostToPeer[message->m_Entity];
	const auto& entry = serializer.m_Entries[message->m_ComponentId];
	entry.m_Remove(m_EntityWorld, peerHandle);
}

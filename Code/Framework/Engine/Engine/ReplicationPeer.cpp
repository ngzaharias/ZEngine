#include "EnginePCH.h"
#include "Engine/ReplicationPeer.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "Engine/ComponentRegistry.h"
#include "Engine/NetworkManager.h"
#include "Engine/ReplicationComponent.h"
#include "Network/Messages.h"
#include "Network/Peer.h"
#include "Network/PeerId.h"

net::ReplicationPeer::ReplicationPeer(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
}

void net::ReplicationPeer::Initialise()
{
	PROFILE_FUNCTION();

	auto& networkManager = m_EntityWorld.WriteResource<eng::NetworkManager>();
	auto& peer = networkManager.GetPeer();

	m_Collections =
	{
		peer.m_OnProcessMessages.Connect(*this, &net::ReplicationPeer::OnProcessMessages),
	};
}

void net::ReplicationPeer::Shutdown()
{
	PROFILE_FUNCTION();
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

void net::ReplicationPeer::OnProcessMessages(const Array<const net::Message*>& messages)
{
	for (const net::Message* message : messages)
	{
		switch (message->m_Type)
		{
		case net::EMessage::DebugCommand:
		{
			const auto* command = static_cast<const net::DebugCommandMessage*>(message);
			Z_LOG(ELog::Debug, "{}", command->m_Data);
		} break;
		}
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
	const auto& componentRegistry = networkManager.GetComponentRegistry();

	const ecs::Entity& peerHandle = m_HostToPeer[message->m_Entity];
	const net::ComponentEntry& entry = componentRegistry.m_Entries.Get(message->m_ComponentId);
	entry.m_Add(m_EntityWorld, peerHandle, message->m_Data);
}

void net::ReplicationPeer::OnUpdateComponent(const net::UpdateComponentMessage* message)
{
	Z_LOG(ELog::Network, "Peer: UpdateComponentMessage");
	Z_ASSERT(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity [{}] doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& networkManager = m_EntityWorld.ReadResource<eng::NetworkManager>();
	const auto& componentRegistry = networkManager.GetComponentRegistry();

	const ecs::Entity& peerHandle = m_HostToPeer[message->m_Entity];
	const net::ComponentEntry& entry = componentRegistry.m_Entries.Get(message->m_ComponentId);
	entry.m_Write(m_EntityWorld, peerHandle, message->m_Data);
}

void net::ReplicationPeer::OnRemoveComponent(const net::RemoveComponentMessage* message)
{
	Z_LOG(ELog::Network, "Peer: RemoveComponentMessage");
	Z_ASSERT(enumerate::Contains(m_HostToPeer, message->m_Entity), "Entity [{}] doesn't exist on peer!", message->m_Entity.m_Value);

	const auto& networkManager = m_EntityWorld.ReadResource<eng::NetworkManager>();
	const auto& componentRegistry = networkManager.GetComponentRegistry();

	const ecs::Entity& peerHandle = m_HostToPeer[message->m_Entity];
	const net::ComponentEntry& entry = componentRegistry.m_Entries.Get(message->m_ComponentId);
	entry.m_Remove(m_EntityWorld, peerHandle);
}

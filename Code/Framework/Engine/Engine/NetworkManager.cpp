#include "EnginePCH.h"
#include "Engine/NetworkManager.h"

#include "Core/Profiler.h"
#include "ECS/Messages.h"
#include "Network/Host.h"
#include "Network/Messages.h"
#include "Network/Peer.h"

eng::NetworkManager::NetworkManager()
	: m_Host(m_Factory)
	, m_Peer(m_Factory)
{
}

void eng::NetworkManager::Initialise()
{
	m_Factory.Register<net::PeerHandshakeMessage>(net::EMessage::PeerHandshake);
	m_Factory.Register<ecs::EntityCreateMessage>(ecs::EMessage::EntityCreate);
	m_Factory.Register<ecs::EntityDestroyMessage>(ecs::EMessage::EntityDestroy);
	m_Factory.Register<ecs::ComponentAddMessage>(ecs::EMessage::ComponentAdd);
	m_Factory.Register<ecs::ComponentUpdateMessage>(ecs::EMessage::ComponentUpdate);
	m_Factory.Register<ecs::ComponentRemoveMessage>(ecs::EMessage::ComponentRemove);
	m_Factory.Register<ecs::EventAddMessage>(ecs::EMessage::EventAdd);
}

void eng::NetworkManager::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// host first, then peer
	m_Host.Update(gameTime);
	m_Peer.Update(gameTime);
}
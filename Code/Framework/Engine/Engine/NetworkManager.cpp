#include "EnginePCH.h"
#include "Engine/NetworkManager.h"

eng::NetworkManager::NetworkManager()
	: m_Host(m_MessageFactory)
	, m_Peer(m_MessageFactory)
{
	m_MessageFactory.Register<net::DebugCommandMessage>(net::EMessage::DebugCommand);
}

void eng::NetworkManager::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// host first, then peer
	m_Host.Update(gameTime);
	m_Peer.Update(gameTime);
}

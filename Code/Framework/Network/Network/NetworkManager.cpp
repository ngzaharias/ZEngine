#include "Network/NetworkManager.h"

#include "Core/Profiler.h"
#include "Network/Host.h"
#include "Network/Peer.h"

net::NetworkManager::NetworkManager()
	: m_Host(m_MessageFactory)
	, m_Peer(m_MessageFactory)
{
}

void net::NetworkManager::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// host first, then peer
	m_Host.Update(gameTime);
	m_Peer.Update(gameTime);
}

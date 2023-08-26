#include "EnginePCH.h"
#include "Engine/NetworkManager.h"

eng::NetworkManager::NetworkManager(net::ComponentSerializer& serializer)
	: m_Serializer(serializer)
	, m_Host(m_Adaptor, m_Config)
	, m_Peer(m_Adaptor, m_Config)
{
}

void eng::NetworkManager::Update(const GameTime& gameTime)
{
	// host first, then peer
	m_Peer.Update(gameTime);
	m_Host.Update(gameTime);
}

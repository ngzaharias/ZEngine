#include "EnginePCH.h"
#include "Engine/NetworkManager.h"

eng::NetworkManager::NetworkManager()
	: m_Host(m_Adaptor, m_Config)
	, m_Peer(m_Adaptor, m_Config)
{
}

void eng::NetworkManager::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// host first, then peer
	m_Host.Update(gameTime);
	m_Peer.Update(gameTime);
}

#include "EnginePCH.h"
#include "Engine/NetworkManager.h"

eng::NetworkManager::NetworkManager()
	: m_Host()
	, m_Peer()
{
}

void eng::NetworkManager::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// host first, then peer
	m_Host.Update(gameTime);
	m_Peer.Update(gameTime);
}

#include "Network/Adaptor.h"

#include "Network/Messages.h"
#include "Network/Host.h"
#include "Network/PeerId.h"

void* net::Adaptor::CreateMessageFactory()
{
	return nullptr;
}

void net::Adaptor::OnServerClientConnected(int32 clientIndex)
{
	const net::PeerId peerId = net::PeerId(clientIndex);
	m_OnServerClientConnected.Publish(peerId);
}

void net::Adaptor::OnServerClientDisconnected(int32 clientIndex) 
{
	const net::PeerId peerId = net::PeerId(clientIndex);
	m_OnServerClientDisconnected.Publish(peerId);
}
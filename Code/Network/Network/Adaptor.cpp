#include "Network/Adaptor.h"

#include "Network/Messages.h"
#include "Network/Host.h"
#include "Network/PeerId.h"

#include <yojimbo/yojimbo.h>

yojimbo::MessageFactory* net::Adaptor::CreateMessageFactory(yojimbo::Allocator& allocator)
{
	return YOJIMBO_NEW(allocator, net::EngineMessageFactory, allocator);
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
#include "Network/Host.h"

#include "Core/Assert.h"
#include "Core/GameTime.h"
#include "Network/Adaptor.h"
#include "Network/Config.h"
#include "Network/UserId.h"

// https://github.com/ValveSoftware/GameNetworkingSockets
// https://keithjohnston.wordpress.com/2014/02/17/nat-punch-through-for-multiplayer-games/

namespace
{
	constexpr int32 s_ChannelIndex = EnumToValue(net::EChannel::Reliable);
}

net::Host::Host(net::Adaptor& adaptor, net::Config& config)
	: m_Adaptor(adaptor)
	, m_Config(config)
{
}

void net::Host::Startup(const str::String& ipAddress, const int32 port, const float time)
{
	m_Collection =
	{
		m_Adaptor.m_OnServerClientConnected.Connect(*this, &net::Host::OnClientConnected),
		m_Adaptor.m_OnServerClientDisconnected.Connect(*this, &net::Host::OnClientDisconnected)
	};
}

void net::Host::Shutdown()
{
	m_Collection.Disconnect();
}

void net::Host::Update(const GameTime& gameTime)
{
}

void net::Host::SendMessage(const net::PeerId& peerId, void* message)
{
}

void net::Host::ProcessMessage(const net::PeerId& peerId, const void* message)
{
}

void net::Host::OnClientConnected(const net::PeerId& peerId)
{
	Z_LOG(ELog::Network, "Host: Peer [{}] connected.", peerId.m_Value);
	m_PeersConnected.Add(peerId);
}

void net::Host::OnClientDisconnected(const net::PeerId& peerId)
{
	Z_LOG(ELog::Network, "Host: Peer [{}] disconnected.", peerId.m_Value);
	m_PeersConnected.Remove(peerId);
}
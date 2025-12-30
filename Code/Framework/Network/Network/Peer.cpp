#include "Network/Peer.h"

#include "Core/Assert.h"
#include "Core/GameTime.h"
#include "Core/Profiler.h"
#include "Network/Adaptor.h"
#include "Network/Config.h"

namespace
{
	constexpr int32 s_ChannelIndex = EnumToValue(net::EChannel::Reliable);
}

net::Peer::Peer(net::Adaptor& adaptor, net::Config& config)
	: m_Adaptor(adaptor)
	, m_Config(config)
{
}

void net::Peer::Startup(const str::String& ipAddress, const int32 port, const float time)
{
	PROFILE_FUNCTION();

	m_Collection =
	{
		m_Adaptor.m_OnServerClientConnected.Connect(*this, &net::Peer::OnClientConnected),
		m_Adaptor.m_OnServerClientDisconnected.Connect(*this, &net::Peer::OnClientDisconnected),
	};
}

void net::Peer::Shutdown()
{
	PROFILE_FUNCTION();

	m_Collection.Disconnect();

	if (IsRunning())
	{
		if (IsConnected() || IsConnecting())
			Disconnect();
	}
}

void net::Peer::Connect(const str::String& ipAddress, const int32 port)
{
}

void net::Peer::Disconnect()
{
}

void net::Peer::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();
}

void net::Peer::SendMessage(void* message)
{
}

void net::Peer::ProcessMessage(const void* message)
{
}

void net::Peer::OnClientConnected(const PeerId& peerId)
{
	Z_LOG(ELog::Network, "Peer: Connected to server.");
}

void net::Peer::OnClientDisconnected(const PeerId& peerId)
{
	Z_LOG(ELog::Network, "Peer: Disconnected from server.");
}
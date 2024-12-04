#include "Network/Host.h"

#include "Core/Assert.h"
#include "Core/GameTime.h"
#include "Network/Adaptor.h"
#include "Network/Config.h"
#include "Network/UserId.h"

#include <yojimbo/yojimbo.h>

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
	Z_ASSERT(!IsRunning(), "");
	Z_ASSERT(m_MaxPeers <= yojimbo::MaxClients, "Host supports up to a maximum of 64 clients!");

	yojimbo::Address address(ipAddress.c_str(), port);
	m_Server = new yojimbo::Server(yojimbo::GetDefaultAllocator(), s_DefaultPrivateKey, address, m_Config, m_Adaptor, time);
	m_Server->Start(m_MaxPeers);
	Z_LOG(ELog::Network, "Host: Started server on port {} (insecure)", s_DefaultServerPort);

	char addressString[256];
	m_Server->GetAddress().ToString(addressString, sizeof(addressString));
	Z_LOG(ELog::Network, "Host: Server address is {}", addressString);

	m_Collection =
	{
		m_Adaptor.m_OnServerClientConnected.Connect(*this, &net::Host::OnClientConnected),
		m_Adaptor.m_OnServerClientDisconnected.Connect(*this, &net::Host::OnClientDisconnected)
	};
}

void net::Host::Shutdown()
{
	m_Collection.Disconnect();

	if (IsRunning())
	{
		m_Server->Stop();

		delete m_Server;
		m_Server = nullptr;

		Z_LOG(ELog::Network, "Host: Stopped server on port {} (insecure)\n", s_DefaultServerPort);
	}
}

void net::Host::Update(const GameTime& gameTime)
{
	if (IsRunning())
	{
		m_Server->SendPackets();
		m_Server->ReceivePackets();
		for (const net::PeerId peerId : m_PeersConnected)
		{
			for (int32 channel = 0; channel < m_Config.numChannels; ++channel)
			{
				yojimbo::Message* message = m_Server->ReceiveMessage(peerId.m_Value, channel);
				while (message)
				{
					// #todo: delete
					ProcessMessage(peerId, message);

					m_OnProcessMessage.Publish(peerId, message);
					m_Server->ReleaseMessage(peerId.m_Value, message);
					message = m_Server->ReceiveMessage(peerId.m_Value, channel);
				}
			}
		}

		m_Server->AdvanceTime(gameTime.m_TotalTime);
	}
}

void net::Host::SendMessage(const net::PeerId& peerId, yojimbo::Message* message)
{
	m_Server->SendMessage(peerId.m_Value, s_ChannelIndex, message);
}

void net::Host::ProcessMessage(const net::PeerId& peerId, const yojimbo::Message* message)
{
	const net::EMessage messageType = ValueToEnum<net::EMessage>(message->GetType());
	switch (messageType)
	{
	case net::EMessage::ServerCommand:
	{
		const auto* tmessage = dynamic_cast<const net::ServerCommandMessage*>(message);
		if (tmessage->m_Data == net::ECommand::Ping)
		{
			Z_LOG(ELog::Network, "Host: Ping!");
			for (const PeerId peerId : m_PeersConnected)
			{
				auto* response = CreateMessage<ServerCommandMessage>(peerId, EMessage::ServerCommand);
				response->m_Data = ECommand::Ping;
				SendMessage(peerId, response);
			}
		}
	} break;
	}
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
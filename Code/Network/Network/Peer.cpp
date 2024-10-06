#include "Network/Peer.h"

#include "Core/Assert.h"
#include "Core/GameTime.h"
#include "Network/Adaptor.h"
#include "Network/Config.h"

#include <yojimbo/yojimbo.h>

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
	Z_ASSERT(!IsRunning(), "");

	yojimbo::Address address(ipAddress.c_str(), port);
	m_Client = new yojimbo::Client(yojimbo::GetDefaultAllocator(), address, m_Config, m_Adaptor, time);

	m_Connections.Append(m_Adaptor.m_OnServerClientConnected.Connect(*this, &net::Peer::OnClientConnected));
	m_Connections.Append(m_Adaptor.m_OnServerClientDisconnected.Connect(*this, &net::Peer::OnClientDisconnected));
}

void net::Peer::Shutdown()
{
	if (IsRunning())
	{
		m_Adaptor.m_OnServerClientConnected.Disconnect(m_Connections[0]);
		m_Adaptor.m_OnServerClientDisconnected.Disconnect(m_Connections[1]);
		m_Connections.RemoveAll();

		if (IsConnected() || IsConnecting())
			Disconnect();

		delete m_Client;
		m_Client = nullptr;
	}
}

void net::Peer::Connect(const str::String& ipAddress, const int32 port)
{
	Z_ASSERT(IsRunning(), "");

	uint64_t clientId;
	yojimbo::random_bytes((uint8_t*)&clientId, 8);

	yojimbo::Address address(ipAddress.c_str(), port);
	m_Client->InsecureConnect(s_DefaultPrivateKey, clientId, address);
	Z_LOG(ELog::Network, "Peer: InsecureConnect");
}

void net::Peer::Disconnect()
{
	if (IsRunning() && (IsConnected() || IsConnecting()))
	{
		m_Client->Disconnect();
		Z_LOG(ELog::Network, "Peer: Disconnected");
	}
}

void net::Peer::Update(const GameTime& gameTime)
{
	if (IsRunning())
	{
		m_Client->SendPackets();
		m_Client->ReceivePackets();

		if (m_Client->IsConnected())
		{
			for (int32 channel = 0; channel < m_Config.numChannels; ++channel)
			{
				yojimbo::Message* message = m_Client->ReceiveMessage(channel);
				while (message)
				{
					// #todo: delete
					ProcessMessage(message);

					m_OnProcessMessage.Publish(message);
					m_Client->ReleaseMessage(message);
					message = m_Client->ReceiveMessage(channel);
				}
			}
		}

		m_Client->AdvanceTime(gameTime.m_TotalTime);
	}
}

void net::Peer::SendMessage(yojimbo::Message* message)
{
	Z_ASSERT(IsRunning(), "");
	Z_ASSERT(m_Client->IsConnected(), "");

	m_Client->SendMessage(s_ChannelIndex, message);
}

void net::Peer::ProcessMessage(const yojimbo::Message* message)
{
	const EMessage messageType = ValueToEnum<EMessage>(message->GetType());
	switch (messageType)
	{
	case EMessage::ServerCommand:
	{
		const auto* tmessage = dynamic_cast<const net::ServerCommandMessage*>(message);
		if (tmessage->m_Data == net::ECommand::Ping)
			Z_LOG(ELog::Debug, "Peer: Ping!");
	} break;
	}
}

void net::Peer::OnClientConnected(const PeerId& peerId)
{
	Z_LOG(ELog::Network, "Peer: Connected to server.");
}

void net::Peer::OnClientDisconnected(const PeerId& peerId)
{
	Z_LOG(ELog::Network, "Peer: Disconnected from server.");
}
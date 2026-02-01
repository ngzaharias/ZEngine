#include "Network/Peer.h"

#include "Core/Assert.h"
#include "Core/GameTime.h"
#include "Core/Profiler.h"
#include "Core/TypeInfo.h"
#include "Network/MessageEnum.h"
#include "Network/Messages.h"

namespace
{
	bool SendMessage(HSteamNetConnection connection, const void* pData, uint32 nSizeOfData)
	{
		int nSendFlags = k_nSteamNetworkingSend_Unreliable;
		EResult res = SteamNetworkingSockets()->SendMessageToConnection(connection, pData, nSizeOfData, nSendFlags, nullptr);
		switch (res)
		{
		case k_EResultOK:
		case k_EResultIgnored:
			break;

		case k_EResultInvalidParam:
			Z_LOG(ELog::Network, "Peer: Failed sending data to server: Invalid connection handle, or the individual message is too big.\n");
			return false;
		case k_EResultInvalidState:
			Z_LOG(ELog::Network, "Peer: Failed sending data to server: Connection is in an invalid state.\n");
			return false;
		case k_EResultNoConnection:
			Z_LOG(ELog::Network, "Peer: Failed sending data to server: Connection has ended.\n");
			return false;
		case k_EResultLimitExceeded:
			Z_LOG(ELog::Network, "Peer: Failed sending data to server: There was already too much data queued to be sent.\n");
			return false;
		default:
		{
			Z_LOG(ELog::Network, "Peer: SendMessageToConnection returned {}.", static_cast<int32>(res));
			return false;
		}
		}
		return true;
	}
}

net::Peer::Peer(net::Factory& factory)
	: m_Factory(factory)
{
}

void net::Peer::Connect()
{
	PROFILE_FUNCTION();

	if (IsConnected())
		return;

	Z_LOG(ELog::Network, "Peer: Connect.");

	SteamNetworkingIPAddr addr{};
	addr.ParseString("127.0.0.1:27020");
	m_Connection = SteamNetworkingSockets()->ConnectByIPAddress(addr, 0, nullptr);

	//SteamNetworkingIdentity identity;
	//identity.SetSteamID(SteamUser()->GetSteamID());
	//m_Connection = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
}

void net::Peer::Disconnect()
{
	PROFILE_FUNCTION();
	
	if (!IsConnected())
		return;

	OnDisconnected(m_Connection);
}

bool net::Peer::IsConnected() const
{
	return m_Connection != k_HSteamNetConnection_Invalid;
}

net::PeerId net::Peer::GetPeerId() const
{
	return m_PeerId;
}

void net::Peer::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (m_Connection != k_HSteamNetConnection_Invalid)
	{
		SteamNetworkingMessage_t* msgs[32];
		const int32 count = SteamNetworkingSockets()->ReceiveMessagesOnConnection(m_Connection, msgs, 32);
		for (int32 i = 0; i < count; i++)
		{
			SteamNetworkingMessage_t* steamMessage = msgs[i];

			net::EMessage type = net::EMessage::Unassigned;
			m_Buffer.Reset();
			m_Buffer.Write(steamMessage->GetData(), steamMessage->GetSize());
			m_Buffer.Read(type);

			// messages will be released later
			net::Message* message = m_Factory.Request(type);
			m_Factory.Read(*message, m_Buffer);
			m_Queue.Append(message);

			steamMessage->Release();
		}

		// once we've gathered all messages, publish them
		OnProcessMessages(m_Queue);

		// release all messages that were created
		for (const net::Message* message : m_Queue)
			m_Factory.Release(message);
		m_Queue.RemoveAll();
	}
}

void net::Peer::ReleaseMessage(const net::Message* message)
{
	m_Factory.Release(message);
}

void net::Peer::SendMessage(const net::Message* message)
{
	Z_PANIC(message && message->m_Type != 0, "");

	MemBuffer buffer;
	buffer.Write(message->m_Type);
	m_Factory.Write(*message, buffer);

	void* data = buffer.GetData();
	uint32 bytes = buffer.GetBytes();

	::SendMessage(m_Connection, data, bytes);
}

void net::Peer::OnGameJoinRequested(GameRichPresenceJoinRequested_t* pCallback)
{
	Z_LOG(ELog::Network, "Peer: Join Request.");
}

void net::Peer::OnLobbyJoinRequested(GameLobbyJoinRequested_t* pCallback)
{
	Z_LOG(ELog::Network, "Peer: Join Request.");
	CSteamID hostId = pCallback->m_steamIDFriend;

	SteamNetworkingIdentity identity;
	identity.SetSteamID(hostId);
	m_Connection = SteamNetworkingSockets()->ConnectP2P(identity, 0, 0, nullptr);
}

void net::Peer::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback)
{
	const HSteamNetConnection connection = pCallback->m_hConn;
	const SteamNetConnectionInfo_t info = pCallback->m_info;
	if (info.m_hListenSocket)
		return;

	const ESteamNetworkingConnectionState eOldState = pCallback->m_eOldState;
	const ESteamNetworkingConnectionState eNewState = info.m_eState;

	const bool hasConnected =
		eOldState != k_ESteamNetworkingConnectionState_Connected &&
		eNewState == k_ESteamNetworkingConnectionState_Connected;
	if (hasConnected)
	{
		OnConnected(connection);
	}

	const bool hasDisconnected =
		eOldState == k_ESteamNetworkingConnectionState_ClosedByPeer ||
		eNewState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally;
	if (hasDisconnected)
	{
		OnDisconnected(connection);
	}
}

void net::Peer::OnConnected(const HSteamNetConnection connection)
{
	Z_LOG(ELog::Network, "Peer: Connected.");
	m_Connection = connection;
	m_OnConnected.Publish();
}

void net::Peer::OnDisconnected(const HSteamNetConnection connection)
{
	Z_LOG(ELog::Network, "Peer: Disconnected.");
	SteamNetworkingSockets()->CloseConnection(connection, 0, nullptr, false);

	m_PeerId = {};
	m_Connection = k_HSteamNetConnection_Invalid;
	m_OnDisconnected.Publish();
}

void net::Peer::OnProcessMessages(const Array<const net::Message*>& messages)
{
	PROFILE_FUNCTION();

	for (const net::Message* message : messages)
	{
		switch (static_cast<net::EMessage>(message->m_Type))
		{
		case net::EMessage::PeerHandshake:
			OnPeerHandshake(static_cast<const net::PeerHandshakeMessage*>(message));
			break;
		}
	}

	m_OnProcessMessages.Publish(m_Queue);
}

void net::Peer::OnPeerHandshake(const net::PeerHandshakeMessage* message)
{
	PROFILE_FUNCTION();

	m_PeerId = message->m_PeerId;
	Z_LOG(ELog::Network, "Peer: Assigned PeerId {}.", m_PeerId.GetIndex());
}

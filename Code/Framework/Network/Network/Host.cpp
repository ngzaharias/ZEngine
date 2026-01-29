#include "Network/Host.h"

#include "Core/Assert.h"
#include "Core/GameTime.h"
#include "Core/Profiler.h"
#include "Network/MessageEnum.h"
#include "Network/Messages.h"

// https://github.com/ValveSoftware/GameNetworkingSockets
// https://keithjohnston.wordpress.com/2014/02/17/nat-punch-through-for-multiplayer-games/

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
			Z_LOG(ELog::Network, "Host: Failed sending data to server: Invalid connection handle, or the individual message is too big.\n");
			return false;
		case k_EResultInvalidState:
			Z_LOG(ELog::Network, "Host: Failed sending data to server: Connection is in an invalid state.\n");
			return false;
		case k_EResultNoConnection:
			Z_LOG(ELog::Network, "Host: Failed sending data to server: Connection has ended.\n");
			return false;
		case k_EResultLimitExceeded:
			Z_LOG(ELog::Network, "Host: Failed sending data to server: There was already too much data queued to be sent.\n");
			return false;
		default:
		{
			Z_LOG(ELog::Network, "Host: SendMessageToConnection returned {}.", static_cast<int32>(res));
			return false;
		}
		}
		return true;
	}
}

net::Host::Host(net::Factory& factory)
	: m_Factory(factory)
{
}

void net::Host::Startup()
{
	PROFILE_FUNCTION();
	Z_LOG(ELog::Network, "Host: Startup.");

	SteamNetworkingIPAddr addr;
	addr.Clear();
	addr.m_port = 27020;

	m_ListenSocketId = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
	m_ListenSocketIp = SteamNetworkingSockets()->CreateListenSocketIP(addr, 0, nullptr);
	m_NetPollGroup = SteamNetworkingSockets()->CreatePollGroup();

	SteamMatchmaking()->CreateLobby(k_ELobbyTypeFriendsOnly, 4);
}

void net::Host::Shutdown()
{
	PROFILE_FUNCTION();
	Z_LOG(ELog::Network, "Host: Shutdown");

	SteamNetworkingSockets()->CloseListenSocket(m_ListenSocketId);
	SteamNetworkingSockets()->CloseListenSocket(m_ListenSocketIp);
	SteamNetworkingSockets()->DestroyPollGroup(m_NetPollGroup);
	SteamMatchmaking()->LeaveLobby(m_LobbyId);

	m_ListenSocketId = k_HSteamListenSocket_Invalid;
	m_ListenSocketIp = k_HSteamListenSocket_Invalid;
	m_NetPollGroup = k_HSteamNetPollGroup_Invalid;
	m_LobbyId = {};
}

void net::Host::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (m_NetPollGroup != k_HSteamNetPollGroup_Invalid)
	{
		SteamNetworkingMessage_t* msgs[128];
		const int32 count = SteamNetworkingSockets()->ReceiveMessagesOnPollGroup(m_NetPollGroup, msgs, 128);
		for (int32 i = 0; i < count; i++)
		{
			SteamNetworkingMessage_t* steamMessage = msgs[i];

			net::EMessage type = net::EMessage::Unassigned;
			m_Buffer.Reset();
			m_Buffer.Write(steamMessage->GetData(), steamMessage->GetSize());
			m_Buffer.Read(type);

			net::Message* message = m_Factory.Request(type);
			m_Factory.Read(*message, m_Buffer);
			m_Queue.Append(message);

			steamMessage->Release();
		}

		// once we've gathered all messages, publish them
		m_OnProcessMessages.Publish(m_Queue);

		// release all messages that were created
		for (const net::Message* message : m_Queue)
			m_Factory.Release(message);
		m_Queue.RemoveAll();
	}
}

void net::Host::ReleaseMessage(const net::Message* message)
{
	m_Factory.Release(message);
}

void net::Host::BroadcastMessage(const net::Message* message)
{
	Z_PANIC(message && message->m_Type != 0, "");

	MemBuffer buffer;
	buffer.Write(message->m_Type);
	m_Factory.Write(*message, buffer);

	void* data = buffer.GetData();
	uint32 bytes = buffer.GetBytes();
	for (auto&& [peerId, peerData] : m_PeerMap)
	{
		::SendMessage(peerData.m_Connection, data, bytes);
	}
}

void net::Host::SendMessage(const net::PeerId& peerId, const net::Message* message)
{
	Z_PANIC(message && message->m_Type != 0, "");

	MemBuffer buffer;
	buffer.Write(message->m_Type);
	m_Factory.Write(*message, buffer);

	void* data = buffer.GetData();
	uint32 bytes = buffer.GetBytes();

	const net::PeerData& peerData = m_PeerMap.Get(peerId);
	::SendMessage(peerData.m_Connection, data, bytes);
}

void net::Host::OnLobbyCreated(LobbyCreated_t* pCallback)
{
	if (pCallback->m_eResult != k_EResultOK)
		return;

	Z_LOG(ELog::Network, "Host: Lobby Created.");
	m_LobbyId = pCallback->m_ulSteamIDLobby;
	SteamFriends()->ActivateGameOverlayInviteDialog(m_LobbyId);
}

void net::Host::OnNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pCallback)
{
	const HSteamNetConnection connection = pCallback->m_hConn;
	const SteamNetConnectionInfo_t info = pCallback->m_info;
	if (!info.m_hListenSocket)
		return;

	const ESteamNetworkingConnectionState eOldState = pCallback->m_eOldState;
	const ESteamNetworkingConnectionState eNewState = info.m_eState;

	const bool isConnecting =
		eOldState != k_ESteamNetworkingConnectionState_Connecting &&
		eNewState == k_ESteamNetworkingConnectionState_Connecting;
	if (isConnecting)
	{
		const EResult res = SteamNetworkingSockets()->AcceptConnection(connection);
		if (res != k_EResultOK)
		{
			Z_LOG(ELog::Network, "Host: Rejected Peer. {}.", static_cast<int32>(res));
			SteamNetworkingSockets()->CloseConnection(connection, k_ESteamNetConnectionEnd_AppException_Generic, "Failed to accept connection", false);
			return;
		}

		OnPeerConnected(connection);
	}

	const bool isDisconnecting =
		eOldState != k_ESteamNetworkingConnectionState_ClosedByPeer &&
		eNewState == k_ESteamNetworkingConnectionState_ClosedByPeer;
	if (isDisconnecting)
	{
		OnPeerDisconnected(connection);
	}
}

void net::Host::OnPeerConnected(HSteamNetConnection connection)
{
	Z_LOG(ELog::Network, "Host: Accepted Peer.");
	SteamNetworkingSockets()->SetConnectionPollGroup(connection, m_NetPollGroup);

	net::PeerId peerId;
	peerId.m_Value = m_PeerIndex++;

	net::PeerData peerData;
	peerData.m_Connection = connection;
	m_PeerMap.Insert(peerId, peerData);

	// initial message to peer to establish the connection
	auto* message = RequestMessage<net::PeerHandshakeMessage>(net::EMessage::PeerHandshake);
	message->m_PeerId = peerId;

	SendMessage(peerId, message);
	ReleaseMessage(message);

	m_OnPeerConnected.Publish(peerId);
}

void net::Host::OnPeerDisconnected(HSteamNetConnection connection)
{
	for (auto&& [peerId, peerData] : m_PeerMap)
	{
		if (connection != peerData.m_Connection)
			continue;

		m_PeerMap.Remove(peerId);
		m_OnPeerDisconnected.Publish(peerId);
	}
}
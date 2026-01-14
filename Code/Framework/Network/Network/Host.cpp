#include "Network/Host.h"

#include "Core/Assert.h"
#include "Core/GameTime.h"
#include "Core/Profiler.h"

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
}

void net::Host::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (m_NetPollGroup)
	{
		SteamNetworkingMessage_t* msgs[128];
		int numMessages = SteamNetworkingSockets()->ReceiveMessagesOnPollGroup(m_NetPollGroup, msgs, 128);
		for (int i = 0; i < numMessages; i++)
		{
			SteamNetworkingMessage_t* message = msgs[i];
			Z_LOG(ELog::Network, "Host: Received Message.");
			message->Release();
		}
	}
}

void net::Host::BroadcastMessage(void* message)
{
	for (const HSteamNetConnection connection : m_Connections)
	{
		::SendMessage(connection, nullptr, 0);
	}
}

void net::Host::ProcessMessage(const net::PeerId& peerId, const void* message)
{
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

		Z_LOG(ELog::Network, "Host: Accepted Peer.", static_cast<int32>(res));
		SteamNetworkingSockets()->SetConnectionPollGroup(connection, m_NetPollGroup);
		SteamNetworkingSockets()->SendMessageToConnection(connection, nullptr, 0, k_nSteamNetworkingSend_Reliable, nullptr);
		m_Connections.Append(connection);
	}

	const bool isDisconnecting =
		eOldState != k_ESteamNetworkingConnectionState_ClosedByPeer &&
		eNewState == k_ESteamNetworkingConnectionState_ClosedByPeer;
	if (isDisconnecting)
	{
		const auto find = std::find(m_Connections.begin(), m_Connections.end(), connection);
		m_Connections.RemoveAt(find);
	}
}
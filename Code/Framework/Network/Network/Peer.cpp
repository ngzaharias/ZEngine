#include "Network/Peer.h"

#include "Core/Assert.h"
#include "Core/GameTime.h"
#include "Core/Profiler.h"
#include "Core/TypeInfo.h"
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

net::Peer::Peer(net::MessageFactory& messageFactory)
	: m_MessageFactory(messageFactory)
{
}

void net::Peer::Connect()
{
	PROFILE_FUNCTION();
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
	Z_LOG(ELog::Network, "Peer: Disconnect.");

	SteamNetworkingSockets()->CloseConnection(m_Connection, 0, nullptr, false);
	m_Connection = k_HSteamNetConnection_Invalid;
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

			uint32 type = 0;
			m_MessageBuffer.Reset();
			m_MessageBuffer.Write(steamMessage->GetData(), steamMessage->GetSize());
			m_MessageBuffer.Read(type);

			net::Message* message = m_MessageFactory.Request(type);
			m_MessageFactory.Read(*message, m_MessageBuffer);
			m_MessageQueue.Append(message);

			steamMessage->Release();
		}

		// once we've gathered all messages, publish them
		m_OnProcessMessages.Publish(m_MessageQueue);

		// release all messages that were created
		for (const net::Message* message : m_MessageQueue)
			m_MessageFactory.Release(message);
		m_MessageQueue.RemoveAll();
	}
}

void net::Peer::ReleaseMessage(const net::Message* message)
{
	m_MessageFactory.Release(message);
}

void net::Peer::SendMessage(void* message)
{
	::SendMessage(m_Connection, nullptr, 0);
}

void net::Peer::ProcessMessage(const void* message)
{
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
		Z_LOG(ELog::Network, "Peer: Connected.");
		SteamNetworkingSockets()->SendMessageToConnection(connection, nullptr, 0, k_nSteamNetworkingSend_Reliable, nullptr);
	}
}
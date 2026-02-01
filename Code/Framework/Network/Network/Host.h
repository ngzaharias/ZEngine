#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/Set.h"
#include "Core/TypeInfo.h"
#include "Network/Factory.h"
#include "Network/Message.h"
#include "Network/PeerId.h"

#pragma warning(push)
#pragma warning(disable: 4996)
#include "Steam/steam_api.h"
#pragma warning(pop)

class GameTime;

namespace net
{
	struct Message;
}

namespace net
{
	struct PeerData
	{
		HSteamNetConnection m_Connection = k_HSteamNetConnection_Invalid;
	};

	class Host final
	{
		using PeerMap = Map<net::PeerId, net::PeerData>;

	public:
		Host(net::Factory& factory);

		void Startup();
		void Shutdown();

		void Update(const GameTime& gameTime);

		template<typename TMessage>
		TMessage* RequestMessage(const uint32 type);
		template<typename TMessage>
		TMessage* RequestMessage(const net::EMessage type);

		void ReleaseMessage(const net::Message* message);

		void BroadcastMessage(const net::Message* message);
		void SendMessage(const net::PeerId& peerId, const net::Message* message);

		void InviteFriend();

	private:
		STEAM_CALLBACK(net::Host, OnLobbyCreated, LobbyCreated_t);
		STEAM_CALLBACK(net::Host, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);

		void OnPeerConnected(HSteamNetConnection connection);
		void OnPeerDisconnected(HSteamNetConnection connection);

	public:
		Delegate<void()> m_OnHostStartup;
		Delegate<void()> m_OnHostShutdown;
		Delegate<void(const net::PeerId&)> m_OnPeerConnected;
		Delegate<void(const net::PeerId&)> m_OnPeerDisconnected;
		Delegate<void(const Array<const net::Message*>&)> m_OnProcessMessages;

	protected:
		net::Factory& m_Factory;

		MemBuffer m_Buffer = {};
		Array<const net::Message*> m_Queue = {};

		int32 m_PeerIndex = 1;
		PeerMap m_PeerMap = {};

		CSteamID m_LobbyId = {};
		HSteamListenSocket m_ListenSocketId = k_HSteamListenSocket_Invalid;
		HSteamListenSocket m_ListenSocketIp = k_HSteamListenSocket_Invalid;
		HSteamNetPollGroup m_NetPollGroup = k_HSteamNetPollGroup_Invalid;
	};
}

#include "Host.inl"
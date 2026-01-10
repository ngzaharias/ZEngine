#pragma once

#include "Core/Array.h"
#include "Core/Set.h"
#include "Core/String.h"
#include "Core/Types.h"
#include "Network/Messages.h"
#include "Network/PeerId.h"

#pragma warning(push)
#pragma warning(disable: 4996)
#include "Steam/steam_api.h"
#pragma warning(pop)

class GameTime;

namespace net
{
	class Host final
	{
	public:
		void Startup();
		void Shutdown();

		void Update(const GameTime& gameTime);

		void BroadcastMessage(void* message);

	protected:
		void ProcessMessage(const net::PeerId& peerId, const void* message);

	private:
		STEAM_CALLBACK(Host, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);

	protected:
		int32 m_MaxPeers = 64;
		Set<net::PeerId> m_PeersConnected = {};
		Set<net::PeerId> m_PeersDisconnected = {};

		Array<HSteamNetConnection> m_Connections = {};
		HSteamListenSocket m_ListenSocketId = k_HSteamListenSocket_Invalid;
		HSteamListenSocket m_ListenSocketIp = k_HSteamListenSocket_Invalid;
		HSteamNetPollGroup m_NetPollGroup = k_HSteamNetPollGroup_Invalid;
	};
}

#include "Host.inl"
#pragma once

#include "Core/Array.h"
#include "Core/Set.h"
#include "Core/String.h"
#include "Core/TypeInfo.h"
#include "Core/Types.h"
#include "Network/MessageFactory.h"
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
	class Host final
	{
	public:
		Host(net::MessageFactory& messageFactory);

		void Startup();
		void Shutdown();

		void Update(const GameTime& gameTime);

		template<typename TMessage>
		TMessage* RequestMessage(const net::EMessage type);
		void ReleaseMessage(const net::Message* message);

		void BroadcastMessage(const net::Message* message);

	protected:
		void ProcessMessage(const net::PeerId& peerId, const void* message);

	private:
		STEAM_CALLBACK(net::Host, OnLobbyCreated, LobbyCreated_t);
		STEAM_CALLBACK(net::Host, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);

	protected:
		net::MessageFactory& m_MessageFactory;

		Array<HSteamNetConnection> m_Connections = {};

		CSteamID m_LobbyId = {};
		HSteamListenSocket m_ListenSocketId = k_HSteamListenSocket_Invalid;
		HSteamListenSocket m_ListenSocketIp = k_HSteamListenSocket_Invalid;
		HSteamNetPollGroup m_NetPollGroup = k_HSteamNetPollGroup_Invalid;
	};
}

#include "Host.inl"
#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/Set.h"
#include "Core/TypeInfo.h"
#include "Network/Message.h"
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
		TMessage* RequestMessage(const uint32 type);
		template<typename TMessage>
		TMessage* RequestMessage(const net::EMessage type);

		void ReleaseMessage(const net::Message* message);

		void BroadcastMessage(const net::Message* message);

	private:
		STEAM_CALLBACK(net::Host, OnLobbyCreated, LobbyCreated_t);
		STEAM_CALLBACK(net::Host, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);

	public:
		Delegate<void(const Array<const net::Message*>& messages)> m_OnProcessMessages;

	protected:
		MemBuffer m_MessageBuffer;
		net::MessageFactory& m_MessageFactory;
		Array<const net::Message*> m_MessageQueue;

		Array<HSteamNetConnection> m_Connections = {};

		CSteamID m_LobbyId = {};
		HSteamListenSocket m_ListenSocketId = k_HSteamListenSocket_Invalid;
		HSteamListenSocket m_ListenSocketIp = k_HSteamListenSocket_Invalid;
		HSteamNetPollGroup m_NetPollGroup = k_HSteamNetPollGroup_Invalid;
	};
}

#include "Host.inl"
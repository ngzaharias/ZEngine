#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/String.h"
#include "Core/Types.h"
#include "Network/MessageFactory.h"
#include "Network/Messages.h"
#include "Network/PeerId.h"

#pragma warning(push)
#pragma warning(disable: 4996)
#include "Steam/steam_api.h"
#pragma warning(pop)

class GameTime;

namespace net
{
	class Peer final
	{
	public:
		Peer(net::MessageFactory& messageFactory);

		void Connect();
		void Disconnect();

		void Update(const GameTime& gameTime);

		template<typename TMessage>
		TMessage* RequestMessage(const net::EMessage type);
		void ReleaseMessage(const net::Message* message);

		void SendMessage(void* message);

	protected:
		void ProcessMessage(const void* message);

	private:
		STEAM_CALLBACK(net::Peer, OnGameJoinRequested, GameRichPresenceJoinRequested_t);
		STEAM_CALLBACK(net::Peer, OnLobbyJoinRequested, GameLobbyJoinRequested_t);
		STEAM_CALLBACK(net::Peer, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);

	public:
		Delegate<void(const Array<const net::Message*>& messages)> m_OnProcessMessages;

	protected:
		MemBuffer m_MessageBuffer;
		net::MessageFactory& m_MessageFactory;
		Array<const net::Message*> m_MessageQueue;

		HSteamNetConnection m_Connection = k_HSteamNetConnection_Invalid;
	};
}

#include "Peer.inl"
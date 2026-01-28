#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/MemBuffer.h"
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
	class Peer final
	{
	public:
		Peer(net::Factory& factory);

		void Connect();
		void Disconnect();

		bool IsConnected() const;

		void Update(const GameTime& gameTime);

		template<typename TMessage>
		TMessage* RequestMessage(const uint32 type);
		template<typename TMessage>
		TMessage* RequestMessage(const net::EMessage type);

		void ReleaseMessage(const net::Message* message);

		void SendMessage(const net::Message* message);

	private:
		STEAM_CALLBACK(net::Peer, OnGameJoinRequested, GameRichPresenceJoinRequested_t);
		STEAM_CALLBACK(net::Peer, OnLobbyJoinRequested, GameLobbyJoinRequested_t);
		STEAM_CALLBACK(net::Peer, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);

	public:
		Delegate<void(const Array<const net::Message*>& messages)> m_OnProcessMessages;

	protected:
		net::Factory& m_Factory;

		MemBuffer m_Buffer;
		Array<const net::Message*> m_Queue;

		HSteamNetConnection m_Connection = k_HSteamNetConnection_Invalid;
	};
}

#include "Peer.inl"
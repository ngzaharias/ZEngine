#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
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
	class Peer final
	{
	public:
		void Connect();
		void Disconnect();

		void Update(const GameTime& gameTime);

		void SendMessage(void* message);

	protected:
		void ProcessMessage(const void* message);

	private:
		STEAM_CALLBACK(Peer, OnNetConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t);

	protected:
		HSteamNetConnection m_Connection = k_HSteamNetConnection_Invalid;
	};
}

#include "Peer.inl"